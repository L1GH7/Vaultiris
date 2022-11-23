USE [SQLDB]
GO
/****** Object:  StoredProcedure [dbo].[GetSomething]  ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
CREATE PROCEDURE [dbo].[GetSomething] 
	-- Add the parameters for the stored procedure here
	@TypeID int
AS
BEGIN
	-- SET NOCOUNT ON added to prevent extra result sets from
	-- interfering with SELECT statements.
	SET NOCOUNT ON;

    -- Insert statements for procedure here
	SELECT AbcdefgID, AbcdeValue AS ID, AbcdeDesc AS Name, AbcValue, DefValue FROM Table1
	WHERE TypeID = @TypeID
	ORDER BY ID
END
