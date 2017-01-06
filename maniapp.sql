SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";

--
-- Database: `maniapp`
--

-- --------------------------------------------------------

--
-- Tablestructure for table `karma`
--

CREATE TABLE IF NOT EXISTS `karma` (
  `Id` int(11) NOT NULL AUTO_INCREMENT,
  `MapId` mediumint(9) NOT NULL DEFAULT '0',
  `PlayerId` mediumint(9) NOT NULL DEFAULT '0',
  `Score` tinyint(3) NOT NULL DEFAULT '0',
  PRIMARY KEY (`Id`),
  UNIQUE KEY `PlayerId` (`PlayerId`,`MapId`),
  KEY `MapId` (`MapId`),
  KEY `Score` (`Score`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 ;

-- --------------------------------------------------------

--
-- Tablestructure for table `maps`
--

CREATE TABLE IF NOT EXISTS `maps` (
  `Id` mediumint(9) NOT NULL AUTO_INCREMENT,
  `Uid` varchar(27) NOT NULL DEFAULT '',
  `Name` varchar(100) NOT NULL DEFAULT '',
  `Author` varchar(30) NOT NULL DEFAULT '',
  `Environment` varchar(10) NOT NULL DEFAULT '',
  `RoundsJuke` tinyint(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`Id`),
  UNIQUE KEY `Uid` (`Uid`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 ;

-- --------------------------------------------------------

--
-- Tablestructure for table `players`
--

CREATE TABLE IF NOT EXISTS `players` (
  `Id` mediumint(9) NOT NULL AUTO_INCREMENT,
  `Login` varchar(50) NOT NULL DEFAULT '',
  `NickName` varchar(100) DEFAULT NULL,
  `Nation` varchar(150) NOT NULL DEFAULT '',
  `UpdatedAt` datetime NOT NULL DEFAULT '0000-00-00 00:00:00',
  `Wins` mediumint(9) NOT NULL DEFAULT '0',
  `TimePlayed` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`Id`),
  UNIQUE KEY `Login` (`Login`),
  KEY `Nation` (`Nation`),
  KEY `Wins` (`Wins`),
  KEY `UpdatedAt` (`UpdatedAt`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 ;

-- --------------------------------------------------------

--
-- Tablestructure for table `records`
--

CREATE TABLE IF NOT EXISTS `records` (
  `Id` int(11) NOT NULL AUTO_INCREMENT,
  `MapId` mediumint(9) NOT NULL DEFAULT '0',
  `PlayerId` mediumint(9) NOT NULL DEFAULT '0',
  `Score` int(11) NOT NULL DEFAULT '0',
  `Date` datetime NOT NULL DEFAULT '0000-00-00 00:00:00',
  `Checkpoints` text NOT NULL,
  PRIMARY KEY (`Id`),
  UNIQUE KEY `PlayerId` (`PlayerId`,`MapId`),
  KEY `MapId` (`MapId`),
  KEY `Score` (`Score`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 ;

-- --------------------------------------------------------

--
-- Tablestructure for table `times`
--

CREATE TABLE IF NOT EXISTS `times` (
  `Id` int(11) NOT NULL AUTO_INCREMENT,
  `MapId` mediumint(9) NOT NULL DEFAULT '0',
  `PlayerId` mediumint(9) NOT NULL DEFAULT '0',
  `Score` int(11) NOT NULL DEFAULT '0',
  `Date` int(10) unsigned NOT NULL DEFAULT '0',
  `Checkpoints` text NOT NULL,
  PRIMARY KEY (`Id`),
  KEY `PlayerId` (`PlayerId`,`MapId`),
  KEY `MapId` (`MapId`),
  KEY `Score` (`Score`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 ;
