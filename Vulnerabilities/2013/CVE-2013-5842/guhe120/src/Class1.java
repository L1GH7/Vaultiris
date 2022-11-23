import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.io.Serializable;



public class Class1  implements Serializable   {
		
		public int fakeObjAddr_;
	
	
		Class1(int addr) {
			fakeObjAddr_ = addr;
		}
		
		
		private synchronized void writeObject(ObjectOutputStream stream)
		         throws IOException {
			System.out.println("Write " + this.getClass());
			
			TestDeserialize.notifyMainThread();
			
			stream.defaultWriteObjectSlow();
			
			System.out.println("Finished writeObject.");
		}
	}