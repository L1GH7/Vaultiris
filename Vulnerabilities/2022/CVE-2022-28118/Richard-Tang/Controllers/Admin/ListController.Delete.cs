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
        [HttpPost, Route(RouteDelete)]
        public async Task<ActionResult<DeleteResult>> Delete([FromBody] DeleteRequest request)
        {
            if (!await _authManager.HasSitePermissionsAsync(request.SiteId, AdvertisementUtils.PermissionsList))
            {
                return Unauthorized();
            }

            await _advertisementRepository.DeleteAsync(request.SiteId, request.AdvertisementId);

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

            return new DeleteResult
            {
                Advertisements = advertisements
            };
        }
    }
}
