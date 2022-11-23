using System.Text;
using SSCMS.Advertisement.Models;
using System.Threading.Tasks;
using SSCMS.Services;
using SSCMS.Models;
using SSCMS.Utils;

namespace SSCMS.Advertisement.Utils
{
    public class ScriptFloating
    {
        private readonly IPathManager _pathManager;
        private readonly Site _site;
        private readonly string _apiUrl;
        private readonly Models.Advertisement _advertisement;

        public ScriptFloating(IPathManager pathManager, Site site, string apiUrl, Models.Advertisement advertisement)
        {
            _pathManager = pathManager;
            _site = site;
            _apiUrl = apiUrl;
            _advertisement = advertisement;
        }

        public async Task<string> GetScriptAsync()
        {
            var linkUrl = _advertisement.NavigationUrl;
            var imageUrl = await _pathManager.ParseSiteUrlAsync(_site, _advertisement.ImageUrl, false);

            var width = _advertisement.Width == 0 ? 160 : _advertisement.Width;
            var height = _advertisement.Height == 0 ? 600 : _advertisement.Height;
            var closeImageUrl = PageUtils.Combine(_apiUrl, AdvertisementUtils.AssetsUrlClose);

            var closeDiv = _advertisement.IsCloseable
                ? $@"
<div class=""ads-float-close"" style=""width: {width}px; height: 18px; position: absolute; left: 0px; top: {height}px; background: url(&quot;{closeImageUrl}&quot;) right center no-repeat rgb(235, 235, 235); cursor: pointer;"" onclick=""document.getElementById('ad_{_advertisement.Id}').style.display = 'none';""></div>
"
                : string.Empty;

            var position = "left: 0px; top: 65px;";
            if (_advertisement.PositionType == PositionType.LeftTop)
            {
                position = $"left: {_advertisement.PositionX}px; top: {_advertisement.PositionY}px;";
            }
            else if (_advertisement.PositionType == PositionType.LeftBottom)
            {
                position = $"left: {_advertisement.PositionX}px; bottom: {_advertisement.PositionY}px;";
            }
            else if (_advertisement.PositionType == PositionType.RightTop)
            {
                position = $"right: {_advertisement.PositionX}px; top: {_advertisement.PositionY}px;";
            }
            else if (_advertisement.PositionType == PositionType.RightBottom)
            {
                position = $"right: {_advertisement.PositionX}px; bottom: {_advertisement.PositionY}px;";
            }

            var builder = new StringBuilder($@"
<div id=""ad_{_advertisement.Id}"" class=""ads-float ads-float-left"" style=""position: fixed; width: {width}px; height: {height}px; z-index: 10500; display: block; {position}""><div style=""width: {width}px; height: {height}px; position: absolute; left: 0px; top: 0px;""><a href=""{linkUrl}"" target=""_blank""><img src=""{imageUrl}"" style=""width: {width}px; height: {height}px; "" border=""0""></a></div>{closeDiv}</div>
");

            var type = 1;
            if (_advertisement.RollingType == RollingType.FloatingInWindow)
            {
                type = 1;
            }
            else if (_advertisement.RollingType == RollingType.FollowingScreen)
            {
                type = 2;
            }
            else if (_advertisement.RollingType == RollingType.Static)
            {
                type = 3;
            }

            var positionX = string.Empty;
            var positionY = string.Empty;
            if (_advertisement.PositionType == PositionType.LeftTop)
            {
                positionX = _advertisement.PositionX.ToString();
                positionY = _advertisement.PositionY.ToString();
            }
            else if (_advertisement.PositionType == PositionType.LeftBottom)
            {
                positionX = _advertisement.PositionX.ToString();
                positionY =
                    $@"document.body.scrollTop+document.body.offsetHeight-{_advertisement.PositionY}-{_advertisement
                        .Height}";
            }
            else if (_advertisement.PositionType == PositionType.RightTop)
            {
                positionX =
                    $@"document.body.scrollLeft+document.body.offsetWidth-{_advertisement.PositionX}-{_advertisement
                        .Width}";
                positionY = _advertisement.PositionY.ToString();
            }
            else if (_advertisement.PositionType == PositionType.RightBottom)
            {
                positionX =
                    $@"document.body.scrollLeft+document.body.offsetWidth-{_advertisement.PositionX}-{_advertisement
                        .Width}";
                positionY =
                    $@"document.body.scrollTop+document.body.offsetHeight-{_advertisement.PositionY}-{_advertisement
                        .Height}";
            }

            var dateLimited = string.Empty;
            if (_advertisement.IsDateLimited && _advertisement.StartDate != null && _advertisement.EndDate != null)
            {
                dateLimited = $@"
    var sDate{_advertisement.Id} = new Date({_advertisement.StartDate.Value.Year}, {_advertisement.StartDate.Value.Month - 1}, {_advertisement.StartDate.Value.Day}, {_advertisement
                    .StartDate.Value.Hour}, {_advertisement.StartDate.Value.Minute});
    var eDate{_advertisement.Id} = new Date({_advertisement.EndDate.Value.Year}, {_advertisement.EndDate.Value.Month - 1}, {_advertisement.EndDate.Value.Day}, {_advertisement.EndDate.Value.Hour}, {_advertisement.EndDate.Value.Minute});
    ad{_advertisement.Id}.SetDate(sDate{_advertisement.Id}, eDate{_advertisement.Id});
";
            }

            builder.Append($@"
<script type=""text/javascript"">
    var ad{_advertisement.Id}=new Ad_Move(""ad_{_advertisement.Id}"");
    ad{_advertisement.Id}.SetLocation({positionX}, {positionY});
    ad{_advertisement.Id}.SetType({type});
    {dateLimited}
    ad{_advertisement.Id}.Run();
</script>
");

            return builder.ToString();
        }
    }
}
