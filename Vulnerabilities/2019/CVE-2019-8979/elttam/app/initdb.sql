CREATE TABLE IF NOT EXISTS `members` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `username` varchar(32) NOT NULL,
  `first_name` varchar(32) NOT NULL,
  `last_name` varchar(32) NOT NULL,
  `email` varchar(127) DEFAULT NULL,
  `password` varchar(64) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=1;
INSERT INTO `members` (`username`,`first_name`,`last_name`,`email`,`password`) VALUES ('bsmith','Bill','Smith','bsmith@blah.com','p@ssw0rd');
INSERT INTO `members` (`username`,`first_name`,`last_name`,`email`,`password`) VALUES ('asmith','Alice','Smith','asmith@blah.com','1q2w3e4r');
