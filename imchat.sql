/*
Navicat MySQL Data Transfer

Source Server         : localhost_3306
Source Server Version : 50611
Source Host           : localhost:3306
Source Database       : imchat

Target Server Type    : MYSQL
Target Server Version : 50611
File Encoding         : 65001

Date: 2018-04-06 18:58:34
*/

SET FOREIGN_KEY_CHECKS=0;
-- ----------------------------
-- Table structure for `chatrecord`
-- ----------------------------
DROP TABLE IF EXISTS `chatrecord`;
CREATE TABLE `chatrecord` (
  `recordID` int(11) NOT NULL AUTO_INCREMENT,
  `sendID` int(11) DEFAULT NULL,
  `sendName` varchar(50) DEFAULT NULL,
  `receiveID` int(50) DEFAULT NULL,
  `receiveName` varchar(50) DEFAULT NULL,
  `msgContent` varchar(1024) DEFAULT NULL,
  `sendTime` datetime DEFAULT NULL,
  PRIMARY KEY (`recordID`)
) ENGINE=InnoDB AUTO_INCREMENT=278162367 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of chatrecord
-- ----------------------------
INSERT INTO `chatrecord` VALUES ('278162356', '278162354', '小春', '278162355', null, 'oo9WZ+7Euhw=', '2018-04-05 18:41:49');
INSERT INTO `chatrecord` VALUES ('278162357', '278162355', '小丽', '278162354', null, 'o+CLEotj68cIPFU/', '2018-04-05 18:50:21');
INSERT INTO `chatrecord` VALUES ('278162358', '278162354', '小春', '278162355', null, 'p3zGeIV4hv+FPn8gK1FA6stw/9Z79yVGnoVOEXo+SdthlXMgFHOUJBi0nQfgD57NE5kQ9mc5AMuzT3N6Xj0MWM+ol4t5fA1wxlvvig==', '2018-04-05 20:38:15');
INSERT INTO `chatrecord` VALUES ('278162359', '278162354', '小春', '278162355', null, 'AB18YhagUOVa9MNMnWQmG1A2dw98ITG1NxgztDnBiraDUgbfJ0Z/EA==', '2018-04-05 20:38:22');
INSERT INTO `chatrecord` VALUES ('278162360', '278162355', '小丽', '278162354', null, 'zlXzuhAAe99XPWxwWKaqpgDLXxaNXwwhgWZ4weSO6MY=', '2018-04-05 20:38:28');
INSERT INTO `chatrecord` VALUES ('278162361', '278162354', '小春', '278162355', null, 'vgD+jafSCHY=', '2018-04-05 20:38:33');
INSERT INTO `chatrecord` VALUES ('278162362', '278162354', '小春', '278162355', null, 'Lh04stqK1VWXQ16u4+CJsIzaJ5GPXWOZ3IiVy7Zl5rAgefii2FopyQ==', '2018-04-05 20:39:58');
INSERT INTO `chatrecord` VALUES ('278162363', '278162355', '小丽', '278162354', null, 'EkVFG5u4XSWugIlDkfVAMw==', '2018-04-06 13:35:53');
INSERT INTO `chatrecord` VALUES ('278162364', '278162354', '小春', '278162355', null, 'aFQOMFGjPl5TA7L4', '2018-04-06 13:36:09');
INSERT INTO `chatrecord` VALUES ('278162365', '278162354', '小春', '278162355', null, 'cbgtSuFKwRM=', '2018-04-06 14:18:42');
INSERT INTO `chatrecord` VALUES ('278162366', '278162355', '小丽', '278162354', null, 'o+CLEotj68cIPFU/', '2018-04-06 14:18:52');

-- ----------------------------
-- Table structure for `groupinfo`
-- ----------------------------
DROP TABLE IF EXISTS `groupinfo`;
CREATE TABLE `groupinfo` (
  `groupID` int(11) NOT NULL AUTO_INCREMENT,
  `groupName` varchar(50) NOT NULL,
  `masterID` int(11) NOT NULL,
  `maxCount` int(11) DEFAULT NULL,
  `nowCount` int(11) DEFAULT NULL,
  PRIMARY KEY (`groupID`)
) ENGINE=InnoDB AUTO_INCREMENT=888889 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of groupinfo
-- ----------------------------
INSERT INTO `groupinfo` VALUES ('888888', '大学', '278162355', null, null);

-- ----------------------------
-- Table structure for `grouplist`
-- ----------------------------
DROP TABLE IF EXISTS `grouplist`;
CREATE TABLE `grouplist` (
  `gListID` int(11) NOT NULL AUTO_INCREMENT,
  `groupID` int(11) DEFAULT NULL,
  `memberID` int(11) DEFAULT NULL,
  PRIMARY KEY (`gListID`)
) ENGINE=InnoDB AUTO_INCREMENT=20 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of grouplist
-- ----------------------------
INSERT INTO `grouplist` VALUES ('1', '888888', '278162354');
INSERT INTO `grouplist` VALUES ('2', '888888', '278162355');
INSERT INTO `grouplist` VALUES ('3', '888888', '51491');
INSERT INTO `grouplist` VALUES ('4', '888888', '91935');
INSERT INTO `grouplist` VALUES ('5', '888888', '23569');
INSERT INTO `grouplist` VALUES ('6', '888888', '45609');
INSERT INTO `grouplist` VALUES ('7', '888888', '37503');
INSERT INTO `grouplist` VALUES ('8', '888888', '15481');
INSERT INTO `grouplist` VALUES ('9', '888888', '55774');
INSERT INTO `grouplist` VALUES ('10', '888888', '18822');
INSERT INTO `grouplist` VALUES ('11', '888888', '36293');
INSERT INTO `grouplist` VALUES ('12', '888888', '61444');
INSERT INTO `grouplist` VALUES ('13', '888888', '56866');
INSERT INTO `grouplist` VALUES ('14', '888888', '18870');
INSERT INTO `grouplist` VALUES ('15', '888888', '32997');
INSERT INTO `grouplist` VALUES ('16', '888888', '22865');
INSERT INTO `grouplist` VALUES ('17', '888888', '12708');
INSERT INTO `grouplist` VALUES ('18', '888888', '14806');
INSERT INTO `grouplist` VALUES ('19', '888888', '50556');

-- ----------------------------
-- Table structure for `offlinemsg`
-- ----------------------------
DROP TABLE IF EXISTS `offlinemsg`;
CREATE TABLE `offlinemsg` (
  `offMsgID` int(11) NOT NULL AUTO_INCREMENT,
  `sendID` int(11) DEFAULT NULL,
  `sendName` varchar(50) DEFAULT NULL,
  `receiveID` int(50) DEFAULT NULL,
  `receiveName` varchar(50) DEFAULT NULL,
  `msgContent` varchar(1024) DEFAULT NULL,
  `sendTime` datetime DEFAULT NULL,
  PRIMARY KEY (`offMsgID`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of offlinemsg
-- ----------------------------
INSERT INTO `offlinemsg` VALUES ('1', '278162354', '小春', '61444', null, '26wfxM+viIYGOBgZ', '2018-04-06 18:54:36');

-- ----------------------------
-- Table structure for `sysmsg`
-- ----------------------------
DROP TABLE IF EXISTS `sysmsg`;
CREATE TABLE `sysmsg` (
  `sysMsgID` int(11) NOT NULL AUTO_INCREMENT,
  `msgType` varchar(50) DEFAULT NULL,
  `userA` int(11) DEFAULT NULL,
  `userB` int(11) DEFAULT NULL,
  PRIMARY KEY (`sysMsgID`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of sysmsg
-- ----------------------------

-- ----------------------------
-- Table structure for `userfriend`
-- ----------------------------
DROP TABLE IF EXISTS `userfriend`;
CREATE TABLE `userfriend` (
  `userID` int(11) NOT NULL AUTO_INCREMENT,
  `friendID` int(11) NOT NULL,
  PRIMARY KEY (`userID`,`friendID`)
) ENGINE=InnoDB AUTO_INCREMENT=278162356 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of userfriend
-- ----------------------------
INSERT INTO `userfriend` VALUES ('15481', '278162354');
INSERT INTO `userfriend` VALUES ('15481', '278162355');
INSERT INTO `userfriend` VALUES ('32997', '278162354');
INSERT INTO `userfriend` VALUES ('61444', '278162354');
INSERT INTO `userfriend` VALUES ('91935', '278162354');
INSERT INTO `userfriend` VALUES ('278162354', '14806');
INSERT INTO `userfriend` VALUES ('278162355', '278162354');

-- ----------------------------
-- Table structure for `userinfo`
-- ----------------------------
DROP TABLE IF EXISTS `userinfo`;
CREATE TABLE `userinfo` (
  `userID` int(11) NOT NULL AUTO_INCREMENT,
  `userName` varchar(50) DEFAULT NULL,
  `pwd` varchar(50) DEFAULT NULL,
  `userStatus` smallint(6) DEFAULT NULL,
  PRIMARY KEY (`userID`)
) ENGINE=InnoDB AUTO_INCREMENT=278162356 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of userinfo
-- ----------------------------
INSERT INTO `userinfo` VALUES ('12708', '校庆', '123456', null);
INSERT INTO `userinfo` VALUES ('14806', '陈小春', '123456', null);
INSERT INTO `userinfo` VALUES ('15481', '小村', '123456', null);
INSERT INTO `userinfo` VALUES ('18822', '闪烁', '123456', null);
INSERT INTO `userinfo` VALUES ('18870', 'kiki', '123456', null);
INSERT INTO `userinfo` VALUES ('22865', 'dada', '123456', null);
INSERT INTO `userinfo` VALUES ('23569', '小杨', '145236', null);
INSERT INTO `userinfo` VALUES ('32145', '小雪', '123456', null);
INSERT INTO `userinfo` VALUES ('32997', '辉辉', '123456', null);
INSERT INTO `userinfo` VALUES ('36293', 'HAHA', '123456', null);
INSERT INTO `userinfo` VALUES ('37503', '黎明', '123456', null);
INSERT INTO `userinfo` VALUES ('45609', '梨子', '123456', null);
INSERT INTO `userinfo` VALUES ('50556', 'howareyou', '123456', null);
INSERT INTO `userinfo` VALUES ('51491', '小明', '123456', null);
INSERT INTO `userinfo` VALUES ('55774', '刘华', '123456', null);
INSERT INTO `userinfo` VALUES ('56866', '库库', '123456', null);
INSERT INTO `userinfo` VALUES ('61444', 'xixi', '123456', null);
INSERT INTO `userinfo` VALUES ('91935', '小牛', '123456', null);
INSERT INTO `userinfo` VALUES ('278162354', '小春', '123456', null);
INSERT INTO `userinfo` VALUES ('278162355', '小丽', '123456', null);
