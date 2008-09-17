--
-- Table structure for table `source`
--

DROP TABLE IF EXISTS `source`;
CREATE TABLE `source` (
  `id` int(11) NOT NULL auto_increment,
  `data` blob default NULL,
  `comment` varchar(20) default NULL,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=cp1251 COMMENT='Holds binary resources';

--
-- Dumping data for table `source`
--


/*!40000 ALTER TABLE `source` DISABLE KEYS */;
LOCK TABLES `source` WRITE;
INSERT INTO `source` VALUES (1,NULL,'tjar'),(2,NULL,'tjad'),(3,NULL,'demo');
UNLOCK TABLES;
/*!40000 ALTER TABLE `source` ENABLE KEYS */;

