CREATE SCHEMA IF NOT EXISTS modellbasen DEFAULT CHARACTER SET utf8 COLLATE utf8_danish_ci;
GRANT ALL PRIVILEGES ON modellbasen.* TO modellbasen@localhost IDENTIFIED BY 'modellbasen';
FLUSH PRIVILEGES;
