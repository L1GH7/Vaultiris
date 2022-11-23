using System.Collections.Generic;
using System.Threading.Tasks;
using Datory;
using Microsoft.AspNetCore.Mvc;
using SSCMS.Advertisement.Models;
using SSCMS.Advertisement.Utils;
using SSCMS.Utils;

namespace SSCMS.Advertisement.Controllers.Admin
{
    public partial class ListController
    {
        [HttpGet, Route(Route)]
        public async Task<ActionResult<GetResult>> Get([FromQuery] GetRequest request)
        {
            if (!await _authManager.HasSitePermissionsAsync(request.SiteId, AdvertisementUtils.PermissionsList))
            {
                return Unauthorized();
            }

            var types = new List<KeyValuePair<string, string>>
            {
                new KeyValuePair<string, string>(string.Empty, "<所有类型>"),
                new KeyValuePair<string, string>(AdvertisementType.FloatImage.GetValue(),
                    AdvertisementType.FloatImage.GetDisplayName()),
                new KeyValuePair<string, string>(AdvertisementType.ScreenDown.GetValue(),
                    AdvertisementType.ScreenDown.GetDisplayName()),
                new KeyValuePair<string, string>(AdvertisementType.OpenWindow.GetValue(),
                    AdvertisementType.OpenWindow.GetDisplayName())
            };

            var advertisements = string.IsNullOrEmpty(request.AdvertisementType)
                ? await _advertisementRepository.GetAllAsync(request.SiteId)
                : await _advertisementRepository.GetAllAsync(request.SiteId,
                    TranslateUtils.ToEnum(request.AdvertisementType, AdvertisementType.FloatImage));

            foreach (var advertisement in advertisements)
            {
                advertisement.Set("display", await GetDisplayAsync(request.SiteId, advertisement));
                advertisement.Set("scope", advertisement.ScopeType.GetDisplayName());
                advertisement.Set("type", advertisement.AdvertisementType.GetDisplayName());
            }

            return new GetResult
            {
                Advertisements = advertisements,
                Types = types
            };
        }
    }
}
