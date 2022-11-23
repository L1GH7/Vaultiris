package com.hatoan;

import org.springframework.amqp.core.AmqpAdmin;
import org.springframework.amqp.core.Message;
import org.springframework.amqp.core.MessageListener;
import org.springframework.amqp.core.Queue;
import org.springframework.amqp.rabbit.connection.CachingConnectionFactory;
import org.springframework.amqp.rabbit.connection.ConnectionFactory;
import org.springframework.amqp.rabbit.core.RabbitAdmin;
import org.springframework.amqp.rabbit.core.RabbitTemplate;
import org.springframework.amqp.rabbit.listener.SimpleMessageListenerContainer;
import org.springframework.amqp.rabbit.listener.adapter.MessageListenerAdapter;
import org.springframework.amqp.support.converter.SerializerMessageConverter;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

@Configuration
public class RabbitConfiguration {
    @Bean
    public SimpleMessageListenerContainer listenerContainer() {
      SimpleMessageListenerContainer container = new SimpleMessageListenerContainer();
      container.setConnectionFactory(connectionFactory());    
      container.setQueueNames("queue");
      container.setMessageListener(exampleListener());
      return container;
    }
	
    @Bean
    public ConnectionFactory connectionFactory() {
        CachingConnectionFactory connectionFactory =
            new CachingConnectionFactory("localhost");
        return connectionFactory;
    }
    
    @Bean
    public MessageListener exampleListener() {
        return new MessageListener() {
			@Override
			public void onMessage(Message message) {
				try{
					// TODO Auto-generated method stub
					SerializerMessageConverter converter = new SerializerMessageConverter();    		
//		    		Message msg = template.receive("queue");
		        	Object fromMessage = converter.fromMessage(message);	
		        	System.out.println(fromMessage.toString());
				}catch (Exception e){
					//pass
				}
					
			}
        };
    }

    
    @Bean
    public RabbitTemplate rabbitTemplate() {
        return new RabbitTemplate(connectionFactory());
    }


    @Bean
    public AmqpAdmin amqpAdmin() {
        return new RabbitAdmin(connectionFactory());
    }


    @Bean
    public Queue myQueue() {
       return new Queue("queue");
    }
    

}