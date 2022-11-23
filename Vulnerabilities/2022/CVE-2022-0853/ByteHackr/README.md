
### [CVE-2022-0853](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2022-0853)
![](https://img.shields.io/static/v1?label=Product&message=jboss-client&color=blue)
![](https://img.shields.io/static/v1?label=Version&message=NONE&color=blue)
![](https://img.shields.io/static/v1?label=Vulnerability&message=MemoryLeak&color=brighgreen)
# CVE-2022-0853

A memory leak on the jboss client side, when using UserTransaction repeatedly.

Such as:

   private void testTransaction() throws Exception {

     for(int i=1; i< 500000; i++) {
       System.out.println("Starting process " + i);

       // get the UserTransaction and EJB Proxy
       Context ctx = getInitialContext(host, port, username, password);
       UserTransaction tx = getUserTransaction(ctx);
       ControllerRemote cr = (ControllerRemote)
      ctx.lookup("Controller/Controller" + "!com.test.usertransaction.ControllerRemote");

       try {
         tx.begin();
       }
       catch(Exception ex1) {
         ex1.printStackTrace();
       }
       //System.out.println("user transaction started");

       cr.mainCall();

       try {
         tx.commit();;
       }
       catch(Exception ex1) {
         ex1.printStackTrace();
         throw ex1;
       }
       //System.out.println("commited user transaction");

       if(ctx != null)
         ctx.close();
     }
   }


Transaction is executed by a remote client on a EJB deployed in EAP 7.3. Transaction is iterated for 50000 times.  At the end of the iterations memory leakage is observed. 32% memory is occupied by  org.wildfly.transaction.client.provider.remoting.TransactionClientChannel.
