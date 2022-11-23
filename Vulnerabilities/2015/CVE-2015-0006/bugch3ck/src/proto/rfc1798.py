
from pyasn1.type import univ, namedtype
from rfc2251 import *

class CLDAPMessage(univ.Sequence):
	componentType = namedtype.NamedTypes(
		namedtype.NamedType('messageID', MessageID()),
		namedtype.OptionalNamedType('user', LDAPDN()),
		namedtype.NamedType('protocolOp', 
			univ.Choice(componentType=namedtype.NamedTypes(
				namedtype.NamedType('searchRequest', SearchRequest()), 
				namedtype.NamedType('searchResEntry', SearchResultEntry()), 
				namedtype.NamedType('searchResDone', SearchResultDone()), 
				namedtype.NamedType('searchResRef', SearchResultReference()), 
				namedtype.NamedType('abandonRequest', AbandonRequest())
				)
			)
		)
	)

