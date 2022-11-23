
using System.Threading.Tasks;
using SSCMS.Services;
using SSCMS.Models;
using SSCMS.Utils;

namespace SSCMS.Advertisement.Utils
{
    public class ScriptScreenDown
    {
        private readonly IPathManager _pathManager;
        private readonly Site _site;
        private readonly Models.Advertisement _advertisement;

        public ScriptScreenDown(IPathManager pathManager, Site site, Models.Advertisement advertisement)
        {
            _pathManager = pathManager;
            _site = site;
            _advertisement = advertisement;
        }

        public async Task<string> GetScriptAsync()
        {
            var imageUrl = await _pathManager.ParseSiteUrlAsync(_site, _advertisement.ImageUrl, false);

            var sizeString = _advertisement.Width > 0
                ? $"width={_advertisement.Width} "
                : string.Empty;
            sizeString += _advertisement.Height > 0 ? $"height={_advertisement.Height}" : string.Empty;

            return $@"
<script language=""javascript"" type=""text/javascript"">
function ad_changediv(){{
    jQuery('#ad_hiddenLayer_{_advertisement.Id}').slideDown();
    setTimeout(""ad_hidediv()"",{_advertisement.Delay}000);
}}
function ad_hidediv(){{
    jQuery('#ad_hiddenLayer_{_advertisement.Id}').slideUp();
}}
jQuery(document).ready(function(){{
    jQuery('body').prepend('<div id=""ad_hiddenLayer_{_advertisement.Id}"" style=""display: none;""><center><a href=""{_advertisement.NavigationUrl}"" target=""_blank""><img src=""{imageUrl}"" {sizeString} border=""0"" /></a></center></div>');
    setTimeout(""ad_changediv()"",2000);
}});
</script>
";
        }
    }
}
