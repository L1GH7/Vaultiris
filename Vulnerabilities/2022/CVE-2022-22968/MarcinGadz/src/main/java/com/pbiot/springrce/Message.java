package com.pbiot.springrce;

public class Message {
    private String username;
    private String content;

    public Message(String username, String content) {
        this.username = username;
        this.content = content;
    }

    public Message() {
    }

    public String getUsername() {
        return username;
    }

    public void setUsername(String username) {
        this.username = username;
    }

    public String getContent() {
        return content;
    }

    public void setContent(String content) {
        this.content = content;
    }
}
