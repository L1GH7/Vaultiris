/ * *
 * section: Informations
 * synopsis: Extraire des informations sur le domaine Xen 0
 * objectif: démontrer l'utilisation de base de la bibliothèque pour se connecter au
 * hyperviseur et extraire les informations de domaine.
 * utilisation: info1
 * test: info1
 * copie: voir Copyright pour l'état de ce logiciel.
 * /

# include  < stdio.h >
# include  < stdlib.h >
# inclure  < libvirt / libvirt.h >

/ * *
 * getDomainInfo:
 * @name: le nom du domaine
 *
 * extraire les informations du domaine 0
 * /
 vide statique
getDomainInfo ( const  char * uri, const  char * nom)
{
    virConnectPtr conn = NULL ; / * la connexion hyperviseur * /
    virDomainPtr dom = NULL ;   / * le domaine en cours de vérification * /
    virDomainInfo info;        / * les informations récupérées * /
    int ret;

    conn = virConnectOpen (uri);
    if (conn == NULL ) {
        fprintf (stderr, " Impossible de se connecter à l'hyperviseur \ n " );
        erreur goto ;
    }

    / * Trouver le domaine du nom donné * /
    dom = virDomainLookupByName (conn, nom);
    si (dom == NULL ) {
        fprintf (stderr, " Impossible de trouver le domaine % s \ n " , nom);
        erreur goto ;
    }

    / * Obtenir les informations * /
    ret = virDomainGetInfo (dom, & info);
    if (ret < 0 ) {
        fprintf (stderr, " Impossible d'obtenir les informations pour le domaine % s \ n " , nom);
        erreur goto ;
    }
    
    / * Obtenir des informations Iface * /
    virDomainInterfacePtr * ifaces = NULL ;
    int ifaces_count = 0 ;
    taille_t i, j;

    if ((ifaces_count = virDomainInterfaceAddresses (dom, & ifaces, VIR_DOMAIN_INTERFACE_ADDRESSES_SRC_AGENT, 0 )) < 0 )
        goto cleanup;
    
    pour (i = 0 ; i <ifaces_count; i ++) {
        printf ( " nom: % s " , ifaces [i] -> nom );
        if (ifaces [i] -> hwaddr )
            printf ( " hwaddr: % s " , ifaces [i] -> hwaddr );

        pour (j = 0 ; j <ifaces [i] -> naddrs ; j ++) {
            virDomainIPAddressPtr ip_addr = ifaces [i] -> addrs + j;
            printf ( " [addr: % s préfixe: % d type: % d ] " ,
                ip_addr-> addr , ip_addr-> prefix , ip_addr-> type );
    }
    printf ( " \ n " );
}

nettoyer:
    if (ifaces && ifaces_count> 0 )
        pour (i = 0 ; i <ifaces_count; i ++)
            virDomainInterfaceFree (ifaces [i]);
    gratuit (ifaces);

    printf ( " Domaine % s : % d CPU \ n " , nom, info. nrVirtCpu );

 Erreur:
    si (dom! = NULL )
        virDomainFree (dom);
    si (conn! = NULL )
        virConnectClose (conn);
}

int  main ( int argc, char ** argv)
{
    si (argc! = 3 ) {
        fprintf (stderr, " syntaxe: % s : NOM URI \ n " , argv [ 0 ]);
        return  1 ;
    }
    getDomainInfo (argv [ 1 ], argv [ 2 ]);

    return  0 ;
}
