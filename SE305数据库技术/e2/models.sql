-- MySQL Script generated by MySQL Workbench
-- Tue Oct  9 16:21:00 2018
-- Model: New Model    Version: 1.0
-- MySQL Workbench Forward Engineering


SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION';

-- -----------------------------------------------------
-- Schema mydb
-- -----------------------------------------------------

-- -----------------------------------------------------
-- Schema mydb
-- -----------------------------------------------------
CREATE SCHEMA IF NOT EXISTS `mydb` DEFAULT CHARACTER SET utf8 ;
USE `mydb` ;

-- -----------------------------------------------------
-- Table `mydb`.`customer`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `mydb`.`customer` ;

CREATE TABLE IF NOT EXISTS `mydb`.`customer` (
  `id` VARCHAR(16) NOT NULL,
  `password` VARCHAR(16) NOT NULL,
  `name` VARCHAR(45) NOT NULL,
  `sex` ENUM("男", "女") NOT NULL,
  `age` INT NOT NULL,
  `tel` DECIMAL(8) NOT NULL,			#纯数字
  `points` INT NULL,
  PRIMARY KEY (`id`))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `mydb`.`commodity`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `mydb`.`commodity` ;

CREATE TABLE IF NOT EXISTS `mydb`.`commodity` (
  `name` VARCHAR(45) NOT NULL,
  `typeName` VARCHAR(45) NOT NULL,					
  `productionTime` DATETIME NOT NULL,
  `qualityPeriod` DATETIME NOT NULL,
  `price` INT NOT NULL,
  `discount` REAL NOT NULL,
  `amount` INT NOT NULL,
  `supplierId` VARCHAR(16) NOT NULL,
  PRIMARY KEY (`name`),
  CONSTRAINT `supplier`
    FOREIGN KEY (`supplierId`)
    REFERENCES `mydb`.`supplier` (`id`)
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `mydb`.`orderList`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `mydb`.`orderList` ;

CREATE TABLE IF NOT EXISTS `mydb`.`orderList` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `customer` VARCHAR(16) NOT NULL,
  `tel` DECIMAL(8) NOT NULL,
  `address` VARCHAR(45) NOT NULL,
  `state` ENUM("未处理", "已发货", "已完成", "已退货") NOT NULL DEFAULT '未处理',
  `orderDate` DATETIME NOT NULL DEFAULT NOW(),
  `mailDate` DATETIME NULL,
  `reason` VARCHAR(200) NULL,
  `supplierId` VARCHAR(16) NOT NULL,
  PRIMARY KEY (`id`),
  INDEX `customer_idx` (`customer` ASC) VISIBLE,
  CONSTRAINT `customer`
    FOREIGN KEY (`customer`)
    REFERENCES `mydb`.`customer` (`id`)
    ON DELETE CASCADE
    ON UPDATE CASCADE,
  CONSTRAINT `supplierId`
    FOREIGN KEY (`supplierId`)
    REFERENCES `mydb`.`supplier` (`id`)
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `mydb`.`comment`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `mydb`.`comment` ;

CREATE TABLE IF NOT EXISTS `mydb`.`comment` (
  `orderListId` INT NOT NULL,
  `comment` VARCHAR(200) NOT NULL,
  `commentTime` DATETIME NOT NULL,
  `score` REAL NOT NULL,
  INDEX `fk_comment_orderList1_idx` (`orderListId` ASC) VISIBLE,
  PRIMARY KEY (`orderListId`),
  CONSTRAINT `id`
    FOREIGN KEY (`orderListId`)
    REFERENCES `mydb`.`orderList` (`id`)
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `mydb`.`supplier`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `mydb`.`supplier` ;

CREATE TABLE IF NOT EXISTS `mydb`.`supplier` (
  `id` VARCHAR(16) NOT NULL,
  `password` VARCHAR(16) NOT NULL,
  `name` VARCHAR(45) NOT NULL,
  `sex` ENUM("男", "女") NOT NULL,
  `age` INT NOT NULL,
  `tel` DECIMAL(8) NOT NULL,
  PRIMARY KEY (`id`))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `mydb`.`commodityOrderList`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `mydb`.`commodityOrderList` ;

CREATE TABLE IF NOT EXISTS `mydb`.`commodityOrderList` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `orderId` INT NOT NULL,
  `commodityName` VARCHAR(45) NOT NULL,
  `amount` INT NOT NULL,
  `price` REAL NOT NULL DEFAULT 0,
  PRIMARY KEY (`id`),
  INDEX `commodityName_idx` (`commodityName` ASC) VISIBLE,
  INDEX `orderId_idx` (`orderId` ASC) VISIBLE,
  CONSTRAINT `commodityName`
    FOREIGN KEY (`commodityName`)
    REFERENCES `mydb`.`commodity` (`name`)
    ON DELETE CASCADE
    ON UPDATE CASCADE,
  CONSTRAINT `orderId`
    FOREIGN KEY (`orderId`)
    REFERENCES `mydb`.`orderList` (`id`)
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB;


SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;
