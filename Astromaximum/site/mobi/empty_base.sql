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
  KEY `name` (`name`,`country_id`,`state_id`),
  KEY `country_id` (`country_id`)
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
  UNIQUE KEY `name` (`name`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='Holds country names';

--
-- Dumping data for table `countries`
--


/*!40000 ALTER TABLE `countries` DISABLE KEYS */;
LOCK TABLES `countries` WRITE;
UNLOCK TABLES;
/*!40000 ALTER TABLE `countries` ENABLE KEYS */;

DROP TABLE IF EXISTS `customers`;
CREATE TABLE IF NOT EXISTS `customers` (
  `id` int(11) NOT NULL auto_increment,
  `name` varchar(20) NOT NULL default '',
  `realname` varchar(50) default 'Unknown',
  `hash` varchar(32) NOT NULL,
  `role` tinyint(4) NOT NULL default '2',
  `email` varchar(50) default NULL,
  `subscr_date` date default NULL,
  `paymode_id` int(11) NOT NULL COMMENT 'Payment mode id',
  `city_limit` INT( 11 ) NOT NULL DEFAULT '0' COMMENT 'City limit counter',
  `dlcount0` int(11) NOT NULL default '2' COMMENT 'Midlet counter',
  `dlcount1` int(11) NOT NULL default '8' COMMENT 'City counter',
  `dlcount2` int(11) NOT NULL default '10' COMMENT 'Past years midlet counter',
  `active` tinyint(1) NOT NULL default '1',
  PRIMARY KEY  (`id`),
  UNIQUE KEY `name` (`name`),
  UNIQUE KEY `realname` (`realname`),
  KEY `paymode` (`paymode_id`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 COMMENT='Holds customer data' AUTO_INCREMENT=8 ;

--
-- Dumping data for table `customers`
--


/*!40000 ALTER TABLE `customers` DISABLE KEYS */;
LOCK TABLES `customers` WRITE;
INSERT INTO `customers` (`id`, `name`, `realname`, `hash`, `role`, `email`, `subscr_date`, `paymode_id`, `dlcount0`, `dlcount1`, `dlcount2`, `active`) VALUES
	(1,'vmesiats','Vasyl Mesiats','',0,'kiev.999@gmail.com','2007-10-13', 1, -1,-1,-1, 1),
	(2,'88,'Andrei Ivushkin','',0,'aivushkin@gmail.com','2007-10-13', 1,-1,-1,-1, 1),
	(3,'123456789','Demo','e7f03e6b4f8f54d4699eeef576488243', 1, 'demo@astromaximum.com', '2007-10-13', 1, -1,-1,-1,0);
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
  UNIQUE KEY `year` (`year`,`city_id`)
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
-- Table structure for table `_sundgr`
--

DROP TABLE IF EXISTS `_sundgr`;
CREATE TABLE `_sundgr` (
  `start` datetime NOT NULL,
  `end` datetime NOT NULL,
  `dgr` int(11) NOT NULL,
  PRIMARY KEY  (`start`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Sun degrees for current year';

--
-- Table structure for table `_voc`
--

DROP TABLE IF EXISTS `_voc`;
CREATE TABLE `_voc` (
  `start` datetime NOT NULL,
  `end` datetime NOT NULL,
  PRIMARY KEY  (`start`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='VOC for current year';

--
-- Dumping data for table `vocs`
--


/*!40000 ALTER TABLE `_voc` DISABLE KEYS */;
LOCK TABLES `_voc` WRITE;
UNLOCK TABLES;
/*!40000 ALTER TABLE `_voc` ENABLE KEYS */;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;


--
-- Table structure for table `ipblock`
--

CREATE TABLE IF NOT EXISTS `ipblock` (
  `ip` varchar(16) character set utf8 collate utf8_unicode_ci NOT NULL,
  `uid` int(11) NOT NULL default '0',
  `tm_first` timestamp NOT NULL default '0000-00-00 00:00:00',
  `tm_last` timestamp NOT NULL default CURRENT_TIMESTAMP on update CURRENT_TIMESTAMP,
  `tm_block` timestamp NOT NULL default '0000-00-00 00:00:00',
  `accessed` int(11) NOT NULL default '1',
  `pageid` varchar(16) NOT NULL default '0',
  PRIMARY KEY  (`ip`,`pageid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='Holds ip access data';

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

--
-- Структура таблицы `dic_paymode`
--

DROP TABLE IF EXISTS `dic_paymode`;
CREATE TABLE IF NOT EXISTS `dic_paymode` (
  `id` int(11) NOT NULL COMMENT 'Mode id',
  `name` varchar(20) NOT NULL COMMENT 'Mode description',
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 COMMENT='Payment modes dictionary';

--
-- Дамп данных таблицы `dic_paymode`
--

INSERT INTO `dic_paymode` (`id`, `name`) VALUES
(1, 'No payment'),
(2, 'PayPal'),
(3, 'Cash');

--
-- Структура таблицы `dic_role`
--

DROP TABLE IF EXISTS `dic_role`;
CREATE TABLE IF NOT EXISTS `dic_role` (
  `id` int(11) NOT NULL COMMENT 'Mode id',
  `name` varchar(20) NOT NULL COMMENT 'Mode description',
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 COMMENT='Roles dictionary';

--
-- Дамп данных таблицы `dic_role`
--

INSERT INTO `dic_role` (`id`, `name`) VALUES
(0, 'Administrator'),
(1, 'Demo user'),
(2, 'Customer');

--
-- Структура таблицы `paypal_orders`
--

CREATE TABLE IF NOT EXISTS `paypal_orders` (
  `order_id` int(11) NOT NULL auto_increment,
  `txn_id` varchar(20) NOT NULL,
  `order_date` datetime NOT NULL,
  `order_total` decimal(8,2) NOT NULL,
  `email` varchar(50) NOT NULL,
  `first_name` varchar(50) NOT NULL,
  `last_name` varchar(50) NOT NULL,
  `street` varchar(50) NOT NULL,
  `city` varchar(50) NOT NULL,
  `state` varchar(50) NOT NULL,
  `zip` varchar(15) NOT NULL,
  `country` varchar(50) NOT NULL,
  `item_name` varchar(50) NOT NULL,
  PRIMARY KEY  (`order_id`),
  UNIQUE KEY `txn_id` (`txn_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='Holds Paypal transactions' AUTO_INCREMENT=1 ;