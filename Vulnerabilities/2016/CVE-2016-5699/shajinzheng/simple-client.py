import sys
from urllib2 import urlopen


url = sys.argv[1]

try:
    response = urlopen(url)
    print 'HTTP request succeeded, headers in reply:'
    print response.info()

except ValueError as e:
    print 'HTTP request failed with error message:'
    print e.message

except Exception as e:
    print "Other exceptions not related to our interest"