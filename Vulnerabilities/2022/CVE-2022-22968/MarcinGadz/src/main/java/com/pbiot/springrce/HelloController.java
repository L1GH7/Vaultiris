package com.pbiot.springrce;

import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestMapping;

import java.util.List;

@Controller
@RequestMapping("/message")
public class HelloController {
    @GetMapping
    public String getMessages(Model model) {
        List<Message> messages = List.of(new Message("Jan", "Witam"), new Message("admin", "dzień dobry"));
        model.addAttribute("messages", messages);
        return "hello";
    }
    @PostMapping
    public String add(Message message) {
        return "hello";
    }
}
