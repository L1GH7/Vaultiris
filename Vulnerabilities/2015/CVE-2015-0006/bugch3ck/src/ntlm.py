
import struct
import datetime

SIGNATURE = 'NTLMSSP'

NTLMSSP_NEGOTIATE = 1
NTLMSSP_AUTH = 3
NTLMSSP_CHALLENGE = 2

NTLMSSP_REVISION_W2K3 = 0x0f

# Definition of flags
# TODO: support more flags
NTLMSSP_NEGOTIATE_UNICODE = 0x00000001
NTLMSSP_NEGOTIATE_TARGET_INFO = 0x00800000

# Definition ov AvId contants for AV_PAIR structs
MsvAvEOL = 0x0000
MsvAvNbComputerName = 0x0001
MsvAvNbDomainName = 0x0002
MsvAvDnsComputerName = 0x0003
MsvAvDnsDomainName = 0x0004
MsvAvDnsTreeName = 0x0005
MsvAvFlags = 0x0006
MsvAvTimestamp = 0x0007
MsvAvSingleHost = 0x0008
MsvAvTargetName = 0x0009
MsvChannelBindings = 0x000A

# Parse NTLM message of unknown type
def parse_ntlm(data):

	msg = NtlmMessage()

	if (data[0:8] == SIGNATURE):
		print "bad signature"
		return None

	msg.type = struct.unpack("<I",data[8:12])[0]

	return msg

FILETIME_REF = datetime.datetime(1601, 1, 1, 0, 0, 0)

def ParseFiletime(data):
	t = struct.unpack('<Q',data)[0]
	return FILETIME_REF + datetime.timedelta(microseconds=t/10)

def EncodeFiletime(dt):
	td = dt - FILETIME_REF
	t = td.microseconds + ((td.seconds + td.days * 24 * 3600) * 10**6)
	return struct.pack('<Q', t*10)

class AvPair():

	AvIds = [
		MsvAvEOL,
		MsvAvNbComputerName,
		MsvAvNbDomainName,
		MsvAvDnsComputerName,
		MsvAvDnsDomainName,
		MsvAvDnsTreeName,
		MsvAvFlags,
		MsvAvTimestamp,
		MsvAvSingleHost,
		MsvAvTargetName,
		MsvChannelBindings
	]

	AvIdStrings = [
		MsvAvNbComputerName,
		MsvAvNbDomainName,
		MsvAvDnsComputerName,
		MsvAvDnsDomainName,
		MsvAvDnsTreeName,
		MsvAvTargetName
	]

	def __init__(self, avid, value=''):

		self.avid = avid
		self.value = value

	def encode(self, fmt=None):

		buf = ''
		if self.avid == MsvAvFlags:
			buf = self.value
		if self.avid == MsvAvTimestamp:
			buf = EncodeFiletime(self.value)
		elif self.avid in self.AvIdStrings:
			buf = unicode(self.value).encode('utf-16le')
		else:
			buf = self.value

		data = struct.pack("<HH", self.avid, len(buf))
		data += buf

		if fmt == None:
			return data
		else:
			return data.encode(fmt)

class NtlmMessage:

	def __init__(self):
		self.type = None

class NtlmChallenge(NtlmMessage):

	def __init__(self, target='', flags=0xe2898215, nonce=None, version=(6,1,7601), targetinfo=[]):

		if nonce == None:
			nonce = '1122334455667788'.decode('hex')

		self.type = NTLMSSP_NEGOTIATE
		self.target = target
		self.flags = flags
		self.nonce = nonce
		self.version = version

		self.targetinfo = targetinfo

		# add stop item if it does not exist
		if (len(self.targetinfo) > 0) and (self.targetinfo[-1].avid != MsvAvEOL):
			self.targetinfo.append(AvPair(MsvAvEOL))

		# expand useful flags
		# TODO: add more flags
		self.f_unicode = (self.flags & NTLMSSP_NEGOTIATE_UNICODE)	
		self.f_targetinfo = (self.flags & NTLMSSP_NEGOTIATE_TARGET_INFO)

	def encode(self, fmt=None):

		if self.f_unicode:
			target_buf = unicode(self.target).encode('utf-16le')
		else:
			target_buf = self.target

		target_len = len(target_buf)
		target_off = 56 # start of payload

		info_buf = ''
		if self.f_targetinfo:
			for avpair in self.targetinfo:
				avpair.f_unicode = self.f_unicode
				info_buf += avpair.encode()

		info_len = len(info_buf)
		info_off = target_off + target_len

		data = struct.pack("<8sI", SIGNATURE, NTLMSSP_CHALLENGE)
		data += struct.pack("<HHI", target_len, target_len, target_off)
		data += struct.pack("<I", self.flags)
		data += self.nonce
		data += '\0'*8 # reserved
		data += struct.pack("<HHI", info_len, info_len, info_off)
		data += struct.pack("<BBH3sB", self.version[0], self.version[1], self.version[2], '\0'*3, NTLMSSP_REVISION_W2K3)
		data += target_buf
		data += info_buf

		if fmt == None:
			return data
		else:
			return data.encode(fmt)

if __name__ == "__main__":
	info = [
		AvPair(MsvAvNbDomainName, "TEST"),
		AvPair(MsvAvNbComputerName, "TEST"),
		AvPair(MsvAvDnsDomainName, "test"),
		AvPair(MsvAvDnsComputerName, "test"),
		AvPair(MsvAvDnsTreeName, "test"),
		AvPair(MsvAvTimestamp, datetime.datetime.now()),
		AvPair(MsvAvEOL)
	]
	print NtlmChallenge('TEST', targetinfo=info).encode('hex')

