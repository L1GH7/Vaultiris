import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.io.Serializable;



public class Class2  implements Serializable   {
		
		public String s = "";
	
	
		Class2() {
			
		}
		
		
		private synchronized void writeObject(ObjectOutputStream stream)
		         throws IOException {
			try {Thread.sleep(1000000000);} catch(Exception e) {}
		}
	}