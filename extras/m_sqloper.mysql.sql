CREATE TABLE ircd_opers (
  id bigint(20) NOT nullptr auto_increment,
  username text,
  password text,
  hostname text,
  type text,
  active tinyint(1) NOT nullptr DEFAULT 1,
  PRIMARY KEY  (id)
) ENGINE=MyISAM;
