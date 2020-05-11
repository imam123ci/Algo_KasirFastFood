USE [master]
GO

/****** Object:  Database [FastfoodDB]    Script Date: 5/12/2020 1:42:02 AM ******/
CREATE DATABASE [FastfoodDB]
 CONTAINMENT = NONE
 ON  PRIMARY 
( NAME = N'FastfoodDB', FILENAME = N'/var/opt/mssql/data/FastfoodDB.mdf' , SIZE = 8192KB , MAXSIZE = UNLIMITED, FILEGROWTH = 65536KB )
 LOG ON 
( NAME = N'FastfoodDB_log', FILENAME = N'/var/opt/mssql/data/FastfoodDB_log.ldf' , SIZE = 8192KB , MAXSIZE = 2048GB , FILEGROWTH = 65536KB )
 WITH CATALOG_COLLATION = DATABASE_DEFAULT
GO

IF (1 = FULLTEXTSERVICEPROPERTY('IsFullTextInstalled'))
begin
EXEC [FastfoodDB].[dbo].[sp_fulltext_database] @action = 'enable'
end
GO

ALTER DATABASE [FastfoodDB] SET ANSI_NULL_DEFAULT OFF 
GO

ALTER DATABASE [FastfoodDB] SET ANSI_NULLS OFF 
GO

ALTER DATABASE [FastfoodDB] SET ANSI_PADDING OFF 
GO

ALTER DATABASE [FastfoodDB] SET ANSI_WARNINGS OFF 
GO

ALTER DATABASE [FastfoodDB] SET ARITHABORT OFF 
GO

ALTER DATABASE [FastfoodDB] SET AUTO_CLOSE OFF 
GO

ALTER DATABASE [FastfoodDB] SET AUTO_SHRINK OFF 
GO

ALTER DATABASE [FastfoodDB] SET AUTO_UPDATE_STATISTICS ON 
GO

ALTER DATABASE [FastfoodDB] SET CURSOR_CLOSE_ON_COMMIT OFF 
GO

ALTER DATABASE [FastfoodDB] SET CURSOR_DEFAULT  GLOBAL 
GO

ALTER DATABASE [FastfoodDB] SET CONCAT_NULL_YIELDS_NULL OFF 
GO

ALTER DATABASE [FastfoodDB] SET NUMERIC_ROUNDABORT OFF 
GO

ALTER DATABASE [FastfoodDB] SET QUOTED_IDENTIFIER OFF 
GO

ALTER DATABASE [FastfoodDB] SET RECURSIVE_TRIGGERS OFF 
GO

ALTER DATABASE [FastfoodDB] SET  ENABLE_BROKER 
GO

ALTER DATABASE [FastfoodDB] SET AUTO_UPDATE_STATISTICS_ASYNC OFF 
GO

ALTER DATABASE [FastfoodDB] SET DATE_CORRELATION_OPTIMIZATION OFF 
GO

ALTER DATABASE [FastfoodDB] SET TRUSTWORTHY OFF 
GO

ALTER DATABASE [FastfoodDB] SET ALLOW_SNAPSHOT_ISOLATION OFF 
GO

ALTER DATABASE [FastfoodDB] SET PARAMETERIZATION SIMPLE 
GO

ALTER DATABASE [FastfoodDB] SET READ_COMMITTED_SNAPSHOT OFF 
GO

ALTER DATABASE [FastfoodDB] SET HONOR_BROKER_PRIORITY OFF 
GO

ALTER DATABASE [FastfoodDB] SET RECOVERY FULL 
GO

ALTER DATABASE [FastfoodDB] SET  MULTI_USER 
GO

ALTER DATABASE [FastfoodDB] SET PAGE_VERIFY CHECKSUM  
GO

ALTER DATABASE [FastfoodDB] SET DB_CHAINING OFF 
GO

ALTER DATABASE [FastfoodDB] SET FILESTREAM( NON_TRANSACTED_ACCESS = OFF ) 
GO

ALTER DATABASE [FastfoodDB] SET TARGET_RECOVERY_TIME = 60 SECONDS 
GO

ALTER DATABASE [FastfoodDB] SET DELAYED_DURABILITY = DISABLED 
GO

ALTER DATABASE [FastfoodDB] SET QUERY_STORE = OFF
GO

ALTER DATABASE [FastfoodDB] SET  READ_WRITE 
GO

