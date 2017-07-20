/*
Navicat MySQL Data Transfer

Source Server         : 120.24.249.45
Source Server Version : 50629
Source Host           : 120.24.249.45:3306
Source Database       : yzm_version_test_db

Target Server Type    : MYSQL
Target Server Version : 50629
File Encoding         : 65001

Date: 2017-07-12 09:42:23
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for t_picture
-- ----------------------------
DROP TABLE IF EXISTS `t_picture`;
CREATE TABLE `t_picture` (
  `Fid` int(10) unsigned NOT NULL AUTO_INCREMENT COMMENT '主键',
  `Fpic_url`  varchar(256) NOT NULL DEFAULT '',
  `Fpic_md5`  varchar(32)  NOT NULL  DEFAULT '',
  `Fkey_word` varchar(256)  NOT NULL  DEFAULT '',
  `Ftag_word` varchar(256)  NOT NULL  DEFAULT '',
  `Fpre2DPoseInfo` text  NOT NULL  ,
  `Fpre3DPoseInfo` text  NOT NULL  ,
  `Flabel_count`   tinyint(4)     NOT NULL  DEFAULT '1',
  `Fcreate_time` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  `Fupdate_time` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00' ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`Fid`),
  KEY `pic_name_index` (`Fpic_md5`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;



