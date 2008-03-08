-- MySQL dump 10.10
--
-- Host: localhost    Database: amax
-- ------------------------------------------------------
-- Server version	5.0.22

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `cities`
--

DROP TABLE IF EXISTS `cities`;
CREATE TABLE `cities` (
  `id` int(11) NOT NULL auto_increment,
  `name` varchar(50) NOT NULL default '',
  `country_id` int(11) NOT NULL default '0',
  `state_id` int(11) NOT NULL default '0',
  PRIMARY KEY  (`id`),
  KEY `name` (`name`,`country_id`,`state_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='Holds city names';

--
-- Dumping data for table `cities`
--


/*!40000 ALTER TABLE `cities` DISABLE KEYS */;
LOCK TABLES `cities` WRITE;
UNLOCK TABLES;
/*!40000 ALTER TABLE `cities` ENABLE KEYS */;

--
-- Table structure for table `countries`
--

DROP TABLE IF EXISTS `countries`;
CREATE TABLE `countries` (
  `id` int(11) NOT NULL auto_increment,
  `name` varchar(50) NOT NULL default '',
  `continent` enum('AFR','ASI','EAS','SAS','SEA','CAR','CAM','EUR','EEU','WEE','MIE','NAM','OCE','SAM'),
  PRIMARY KEY  (`id`),
  KEY `name` (`name`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='Holds country names';

--
-- Dumping data for table `countries`
--


/*!40000 ALTER TABLE `countries` DISABLE KEYS */;
LOCK TABLES `countries` WRITE;
UNLOCK TABLES;
/*!40000 ALTER TABLE `countries` ENABLE KEYS */;

--
-- Table structure for table `customers`
--

DROP TABLE IF EXISTS `customers`;
CREATE TABLE `customers` (
  `id` int(11) NOT NULL auto_increment,
  `name` varchar(20) NOT NULL default '',
  `realname` varchar(50) NOT NULL default 'Unknown',
  `hash` varchar(32) default NULL,
  `role` tinyint(4) NOT NULL default '1',
  `email` varchar(50) default NULL,
  `subscr_date` date default NULL,
  `dl_count` int(11) NOT NULL default '2',
  `city_count` int(11) NOT NULL default '5',
  PRIMARY KEY  (`id`),
  KEY `name` (`name`,`realname`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='Holds customer data';

--
-- Dumping data for table `customers`
--


/*!40000 ALTER TABLE `customers` DISABLE KEYS */;
LOCK TABLES `customers` WRITE;
INSERT INTO `customers` VALUES 
	(1,'vmesyats','Vasyl Mesyats','123',0,NULL,NULL,-1),
	(2,'aivushkin','Andrei Ivushkin','65536',0,NULL,NULL,-1),
	(3,'123456789','Demo','012345678',1,NULL,NULL,-1);
UNLOCK TABLES;
/*!40000 ALTER TABLE `customers` ENABLE KEYS */;

--
-- Table structure for table `files`
--

DROP TABLE IF EXISTS `files`;
CREATE TABLE `files` (
  `id` varchar(14) NOT NULL default '',
  `type` VARCHAR(10) NOT NULL,
  `user_id` int(11) NOT NULL default '-1',
  `end_tm` datetime NOT NULL default '0000-00-00 00:00:00',
  `used` binary(1) NOT NULL default 'f',
  `deleted` binary(1) NOT NULL default 'f',
  PRIMARY KEY  (`id`),
  KEY `user_id` (`user_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='Generated files';

--
-- Dumping data for table `files`
--


/*!40000 ALTER TABLE `files` DISABLE KEYS */;
LOCK TABLES `files` WRITE;
UNLOCK TABLES;
/*!40000 ALTER TABLE `files` ENABLE KEYS */;

--
-- Table structure for table `locations`
--

DROP TABLE IF EXISTS `locations`;
CREATE TABLE `locations` (
  `id` int(11) NOT NULL auto_increment,
  `year` smallint(6) NOT NULL default '0',
  `city_id` int(11) NOT NULL default '0',
  `data` blob NOT NULL,
  PRIMARY KEY  (`id`),
  KEY `year` (`year`,`city_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='Holds calculated geodata';

--
-- Dumping data for table `locations`
--


/*!40000 ALTER TABLE `locations` DISABLE KEYS */;
LOCK TABLES `locations` WRITE;
UNLOCK TABLES;
/*!40000 ALTER TABLE `locations` ENABLE KEYS */;

--
-- Table structure for table `source`
--

DROP TABLE IF EXISTS `source`;
CREATE TABLE `source` (
  `id` int(11) NOT NULL auto_increment,
  `data` blob default NULL,
  `comment` varchar(20) default NULL,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='Holds binary resources';

--
-- Dumping data for table `source`
--


/*!40000 ALTER TABLE `source` DISABLE KEYS */;
LOCK TABLES `source` WRITE;
INSERT INTO `source` VALUES (1,NULL,'tjar'),(2,NULL,'tjad'),(3,NULL,'demo');
UNLOCK TABLES;
/*!40000 ALTER TABLE `source` ENABLE KEYS */;

--
-- Table structure for table `sessions`
--

DROP TABLE IF EXISTS `sessions`;
CREATE TABLE `sessions` (
  `name` varchar(32) NOT NULL default '',
  `user_id` int(11) NOT NULL default '-1',
  `tm_start` timestamp NOT NULL default CURRENT_TIMESTAMP on update CURRENT_TIMESTAMP,
  `tm_end` timestamp NOT NULL default '0000-00-00 00:00:00',
  PRIMARY KEY  (`name`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='Durability of user sessions';

--
-- Dumping data for table `sessions`
--


/*!40000 ALTER TABLE `sessions` DISABLE KEYS */;
LOCK TABLES `sessions` WRITE;
UNLOCK TABLES;
/*!40000 ALTER TABLE `sessions` ENABLE KEYS */;

--
-- Table structure for table `states`
--

DROP TABLE IF EXISTS `states`;
CREATE TABLE `states` (
  `id` int(11) NOT NULL auto_increment,
  `name` varchar(50) NOT NULL default '',
  `country_id` int(11) NOT NULL default '0',
  PRIMARY KEY  (`id`),
  KEY `name` (`name`,`country_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='Holds state names';

--
-- Dumping data for table `states`
--


/*!40000 ALTER TABLE `states` DISABLE KEYS */;
LOCK TABLES `states` WRITE;
UNLOCK TABLES;
/*!40000 ALTER TABLE `states` ENABLE KEYS */;

--
-- Table structure for table `vocs`
--

DROP TABLE IF EXISTS `vocs`;
CREATE TABLE `vocs` (
  `start` datetime NOT NULL,
  `end` datetime NOT NULL,
  PRIMARY KEY  (`start`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='VOC for current year';

--
-- Dumping data for table `vocs`
--


/*!40000 ALTER TABLE `vocs` DISABLE KEYS */;
LOCK TABLES `vocs` WRITE;
UNLOCK TABLES;
/*!40000 ALTER TABLE `vocs` ENABLE KEYS */;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;


--
-- Table structure for table `source`
--

DROP TABLE IF EXISTS `source`;
CREATE TABLE `source` (
  `id` int(11) NOT NULL auto_increment,
  `data` blob default NULL,
  `comment` varchar(20) default NULL,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='Holds binary resources';

--
-- Dumping data for table `source`
--


/*!40000 ALTER TABLE `source` DISABLE KEYS */;
LOCK TABLES `source` WRITE;
INSERT INTO `source` VALUES (1,NULL,'tjar'),(2,NULL,'tjad'),(3,NULL,'demo');
UNLOCK TABLES;
/*!40000 ALTER TABLE `source` ENABLE KEYS */;

/*
--
-- Table structure for table `divisions`
--

DROP TABLE IF EXISTS `divisions`;
CREATE TABLE `divisions` (
  `id` int(11) NOT NULL auto_increment,
  `name` varchar(50) default NULL,
  `depth` integer NOT NULL,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='Holds subdividions';

--
-- Dumping data for table `divisions`
--
*/
-- /*!40000 ALTER TABLE `divisions` DISABLE KEYS */;
/*LOCK TABLES `divisions` WRITE;
INSERT INTO `divisions` VALUES (1,'Midwest',1),
			(2,'Mountain',1),
			(3,'Pacific',1),
			(4,'South',1),
			(5,'Northeast',1),
			(6,'Territories',1),
			(6,'Territories',1),
;
UNLOCK TABLES;
*/
-- /*!40000 ALTER TABLE `divisions` ENABLE KEYS; */
