package com.example.demo;

import org.springframework.security.access.prepost.PreAuthorize;
import org.springframework.security.core.Authentication;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RestController;
import javax.servlet.http.HttpSession;

@RestController
public class HelloWorld {

    @GetMapping("/*")
    public String sayHello() {
        return "You Pass the Spring Security!!!";
    }

    @GetMapping("/name")
    public String sayMyName() {
        return "You Pass the Spring Security with SayMyName!!!";
    }

    @GetMapping("/admin/{value}")
    public String sayAdmin(@PathVariable String value, HttpSession session, Authentication authentication) {
        System.out.println("path value:" + value + ".");
        if(authentication!=null &&"admin".equals(authentication.getName())) {
            System.out.println(authentication.getName());
            System.out.println(authentication.getAuthorities().toString());
            return "hello admin";
        }
        if(authentication!=null &&"user".equals(authentication.getName())) {
            return "user you can't pass.";
        }
        else {
            return "go away hacker";
        }
    }

    @PreAuthorize("hasAuthority('ROLE_USER')")
    @GetMapping("/user/{value}")
    public String sayUser(@PathVariable String value, HttpSession session, Authentication authentication) {
        System.out.println("path value:" + value + ".");
        return "hello user!!";
    }



}
