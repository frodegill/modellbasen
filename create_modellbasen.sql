-- MySQL Script generated by MySQL Workbench
-- fr. 18. mars 2016 kl. 17.05 +0100
-- Model: New Model    Version: 1.0
-- MySQL Workbench Forward Engineering

SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='TRADITIONAL,ALLOW_INVALID_DATES';

-- -----------------------------------------------------
-- Schema modellbasen
-- -----------------------------------------------------

-- -----------------------------------------------------
-- Schema modellbasen
-- -----------------------------------------------------
CREATE SCHEMA IF NOT EXISTS `modellbasen` DEFAULT CHARACTER SET utf8 COLLATE utf8_danish_ci ;
USE `modellbasen` ;

-- -----------------------------------------------------
-- Table `modellbasen`.`postcode`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `modellbasen`.`postcode` (
  `postcode` CHAR(4) NOT NULL,
  `place` VARCHAR(255) NULL,
  `latitude` DOUBLE NOT NULL,
  `longitude` DOUBLE NOT NULL,
  PRIMARY KEY (`postcode`))
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_danish_ci;


-- -----------------------------------------------------
-- Table `modellbasen`.`event`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `modellbasen`.`event` (
  `id` INT UNSIGNED NOT NULL AUTO_INCREMENT,
  `starttime` BIGINT NOT NULL,
  `endtime` BIGINT NOT NULL,
  `description` TEXT NOT NULL,
  `location` CHAR(4) NOT NULL,
  `owner` INT UNSIGNED NOT NULL,
  PRIMARY KEY (`id`),
  INDEX `fk_event_owner_idx` (`owner` ASC),
  INDEX `fk_event_location_idx` (`location` ASC),
  CONSTRAINT `fk_event_owner`
    FOREIGN KEY (`owner`)
    REFERENCES `modellbasen`.`user` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_event_location`
    FOREIGN KEY (`location`)
    REFERENCES `modellbasen`.`postcode` (`postcode`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `modellbasen`.`picture`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `modellbasen`.`picture` (
  `id` INT UNSIGNED NOT NULL AUTO_INCREMENT,
  `picture1024` BLOB NOT NULL,
  `picture320` BLOB NOT NULL,
  `picture64` BLOB NOT NULL,
  `owner` INT UNSIGNED NOT NULL,
  `event` INT UNSIGNED NULL,
  PRIMARY KEY (`id`),
  INDEX `fk_picture_owner_idx` (`owner` ASC),
  INDEX `fk_picture_event_idx` (`event` ASC),
  CONSTRAINT `fk_picture_owner`
    FOREIGN KEY (`owner`)
    REFERENCES `modellbasen`.`user` (`id`)
    ON DELETE CASCADE
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_picture_event`
    FOREIGN KEY (`event`)
    REFERENCES `modellbasen`.`event` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `modellbasen`.`user`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `modellbasen`.`user` (
  `id` INT UNSIGNED NOT NULL AUTO_INCREMENT,
  `username` VARCHAR(255) NOT NULL,
  `bcrypt_password_hash` VARCHAR(64) NOT NULL,
  `email` VARCHAR(255) NOT NULL,
  `profile_picture` INT UNSIGNED NULL,
  `home_location` CHAR(4) NOT NULL,
  PRIMARY KEY (`id`),
  INDEX `fk_user_profile_picture_idx` (`profile_picture` ASC),
  UNIQUE INDEX `username_UNIQUE` (`username` ASC),
  INDEX `fk_user_home_location_idx` (`home_location` ASC),
  CONSTRAINT `fk_user_profile_picture`
    FOREIGN KEY (`profile_picture`)
    REFERENCES `modellbasen`.`picture` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_user_home_location`
    FOREIGN KEY (`home_location`)
    REFERENCES `modellbasen`.`postcode` (`postcode`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_danish_ci;


-- -----------------------------------------------------
-- Table `modellbasen`.`uservisit`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `modellbasen`.`uservisit` (
  `id` INT UNSIGNED NOT NULL AUTO_INCREMENT,
  `user` INT UNSIGNED NOT NULL,
  `visitor` INT UNSIGNED NOT NULL,
  `last_visit` BIGINT NOT NULL,
  `visits` INT UNSIGNED NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE INDEX `visit` (`user` ASC, `visitor` ASC),
  INDEX `fk_uservisit_visitor_idx` (`visitor` ASC),
  CONSTRAINT `fk_uservisit_visited`
    FOREIGN KEY (`user`)
    REFERENCES `modellbasen`.`user` (`id`)
    ON DELETE CASCADE
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_uservisit_visitor`
    FOREIGN KEY (`visitor`)
    REFERENCES `modellbasen`.`user` (`id`)
    ON DELETE CASCADE
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `modellbasen`.`message`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `modellbasen`.`message` (
  `id` INT UNSIGNED NOT NULL AUTO_INCREMENT,
  `message` TEXT NOT NULL,
  `sender` INT UNSIGNED NOT NULL,
  `recipient` INT UNSIGNED NOT NULL,
  `sent_time` BIGINT NOT NULL,
  `read_time` BIGINT NOT NULL,
  `replied_time` BIGINT NOT NULL,
  `in_reply_to` INT UNSIGNED NULL,
  `sender_deleted` TINYINT(1) NULL,
  `recipient_deleted` TINYINT(1) NULL,
  PRIMARY KEY (`id`),
  INDEX `fk_message_sender_idx` (`sender` ASC),
  INDEX `fk_message_recipient_idx` (`recipient` ASC),
  CONSTRAINT `fk_message_sender`
    FOREIGN KEY (`sender`)
    REFERENCES `modellbasen`.`user` (`id`)
    ON DELETE CASCADE
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_message_recipient`
    FOREIGN KEY (`recipient`)
    REFERENCES `modellbasen`.`user` (`id`)
    ON DELETE CASCADE
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `modellbasen`.`postcode_distance`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `modellbasen`.`postcode_distance` (
  `source` CHAR(4) NOT NULL,
  `destination` CHAR(4) NOT NULL,
  `distance` DOUBLE NULL,
  PRIMARY KEY (`source`, `destination`),
  INDEX `fk_postcode_source_idx` (`source` ASC),
  INDEX `fk_postcode_destination_idx` (`destination` ASC),
  CONSTRAINT `fk_postcode_source`
    FOREIGN KEY (`source`)
    REFERENCES `modellbasen`.`postcode` (`postcode`)
    ON DELETE CASCADE
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_postcode_destination`
    FOREIGN KEY (`destination`)
    REFERENCES `modellbasen`.`postcode` (`postcode`)
    ON DELETE CASCADE
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `modellbasen`.`eventparticipant`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `modellbasen`.`eventparticipant` (
  `id` INT UNSIGNED NOT NULL AUTO_INCREMENT,
  `participant` INT UNSIGNED NULL,
  `event` INT UNSIGNED NULL,
  PRIMARY KEY (`id`),
  INDEX `fk_eventparticipant_participant_idx` (`participant` ASC),
  INDEX `fk_eventparticipant_event_idx` (`event` ASC),
  CONSTRAINT `fk_eventparticipant_participant`
    FOREIGN KEY (`participant`)
    REFERENCES `modellbasen`.`user` (`id`)
    ON DELETE CASCADE
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_eventparticipant_event`
    FOREIGN KEY (`event`)
    REFERENCES `modellbasen`.`event` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `modellbasen`.`datatype`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `modellbasen`.`datatype` (
  `id` INT UNSIGNED NOT NULL,
  `description` VARCHAR(16) NOT NULL,
  PRIMARY KEY (`id`))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `modellbasen`.`tag`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `modellbasen`.`tag` (
  `id` INT UNSIGNED NOT NULL,
  `name` VARCHAR(255) NOT NULL,
  `insert_datatype` INT UNSIGNED NOT NULL,
  `query_datatype` INT UNSIGNED NOT NULL,
  `parent` INT UNSIGNED NULL,
  `query_only` TINYINT(1) NOT NULL,
  PRIMARY KEY (`id`),
  INDEX `fk_tag_datatype_idx` (`insert_datatype` ASC),
  INDEX `fk_tag_parent_idx` (`parent` ASC),
  INDEX `fk_tag_query_datatype_idx` (`query_datatype` ASC),
  CONSTRAINT `fk_tag_insert_datatype`
    FOREIGN KEY (`insert_datatype`)
    REFERENCES `modellbasen`.`datatype` (`id`)
    ON DELETE CASCADE
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_tag_parent`
    FOREIGN KEY (`parent`)
    REFERENCES `modellbasen`.`tag` (`id`)
    ON DELETE CASCADE
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_tag_query_datatype`
    FOREIGN KEY (`query_datatype`)
    REFERENCES `modellbasen`.`datatype` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_danish_ci;


-- -----------------------------------------------------
-- Table `modellbasen`.`taginstance`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `modellbasen`.`taginstance` (
  `id` INT UNSIGNED NOT NULL AUTO_INCREMENT,
  `stringvalue` VARCHAR(255) NULL,
  `intvalue` INT NULL,
  `timevalue` BIGINT NULL,
  `tag` INT UNSIGNED NOT NULL,
  `owner` INT UNSIGNED NULL,
  `eventparticipant` INT UNSIGNED NULL,
  PRIMARY KEY (`id`),
  INDEX `fk_taginstance_tag_idx` (`tag` ASC),
  INDEX `fk_taginstance_owner_idx` (`owner` ASC),
  INDEX `fk_taginstance_eventparticipant_idx` (`eventparticipant` ASC),
  CONSTRAINT `fk_taginstance_tag`
    FOREIGN KEY (`tag`)
    REFERENCES `modellbasen`.`tag` (`id`)
    ON DELETE CASCADE
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_taginstance_owner`
    FOREIGN KEY (`owner`)
    REFERENCES `modellbasen`.`user` (`id`)
    ON DELETE CASCADE
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_taginstance_eventparticipant`
    FOREIGN KEY (`eventparticipant`)
    REFERENCES `modellbasen`.`eventparticipant` (`id`)
    ON DELETE CASCADE
    ON UPDATE NO ACTION)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_danish_ci;


-- -----------------------------------------------------
-- Table `modellbasen`.`tagvalue`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `modellbasen`.`tagvalue` (
  `id` INT UNSIGNED NOT NULL,
  `value` VARCHAR(255) NULL,
  `pos` INT UNSIGNED NOT NULL,
  `tag` INT UNSIGNED NOT NULL,
  PRIMARY KEY (`id`),
  INDEX `fk_tagvalue_select_tag_idx` (`tag` ASC),
  CONSTRAINT `fk_tagvalue_tag`
    FOREIGN KEY (`tag`)
    REFERENCES `modellbasen`.`tag` (`id`)
    ON DELETE CASCADE
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `modellbasen`.`messageboard`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `modellbasen`.`messageboard` (
  `id` INT UNSIGNED NOT NULL AUTO_INCREMENT,
  `message` TEXT NOT NULL,
  `posted_time` BIGINT NOT NULL,
  `user` INT UNSIGNED NOT NULL,
  PRIMARY KEY (`id`),
  INDEX `fk_messageboard_poster_idx` (`user` ASC),
  CONSTRAINT `fk_messageboard_poster`
    FOREIGN KEY (`user`)
    REFERENCES `modellbasen`.`user` (`id`)
    ON DELETE CASCADE
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;
