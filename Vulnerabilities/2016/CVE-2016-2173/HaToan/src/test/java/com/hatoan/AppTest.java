package com.hatoan;

import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;

import org.springframework.amqp.core.AmqpTemplate;
import org.springframework.amqp.core.Message;
import org.springframework.amqp.core.MessageProperties;
import org.springframework.amqp.support.converter.SerializerMessageConverter;
import org.springframework.context.ApplicationContext;
import org.springframework.context.annotation.AnnotationConfigApplicationContext;

import java.nio.file.Files;
import java.nio.file.Paths;
import java.nio.file.Path;

public class AppTest 
{
	public  static  void main(String[] args) throws FileNotFoundException, IOException, ClassNotFoundException
	{
		Path path1 =  Paths.get("code_reverse_tcp.ser");
		byte[] body_reverse_code = Files.readAllBytes(path1);
		Path path2 =  Paths.get("exe_code_reverse_tcp.ser");
		byte[] body_reverse_exe = Files.readAllBytes(path2);
		
		MessageProperties mp = new MessageProperties();
		mp.setContentType("application/x-java-serialized-object");
        Message msg1 = new Message(body_reverse_code, mp);
        Message msg2 = new Message(body_reverse_exe, mp);
		
		ApplicationContext context =
    		    new AnnotationConfigApplicationContext(RabbitConfiguration.class);
    		AmqpTemplate template = context.getBean(AmqpTemplate.class);
		
	
    	template.send("queue", msg1);
    	template.send("queue", msg2);
    	
    	System.exit(0);
	}
}
