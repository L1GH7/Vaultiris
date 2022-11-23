namespace SSCMS.Advertisement.Utils
{
	public class ScriptOpenWindow
	{
	    private readonly Models.Advertisement _advertisement;

        public ScriptOpenWindow(Models.Advertisement advertisement)
		{
            _advertisement = advertisement;
		}

		public string GetScript()
		{
            var sizeString = _advertisement.Width > 0 ? $",width={_advertisement.Width}"
                : string.Empty;
            sizeString += _advertisement.Height > 0 ? $",height={_advertisement.Height} " : string.Empty;

            return $@"
<script language=""javascript"" type=""text/javascript"">
function ad_open_win_{_advertisement.Id}() {{
	var popUpWin{_advertisement.Id} = open(""{_advertisement.NavigationUrl}"", (window.name!=""popUpWin{_advertisement.Id}"")?""popUpWin{_advertisement.Id}"":"""", ""toolbar=no,location=no,directories=no,resizable=no,copyhistory=yes{sizeString}"");
}}
try{{
	setTimeout(""ad_open_win_{_advertisement.Id}();"",50);
}}catch(e){{}}
</script>
";
        }
	}
}
