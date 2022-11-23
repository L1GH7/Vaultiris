using System;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;
using SSCMS.Advertisement.Models;
using SSCMS.Advertisement.Utils;
using SSCMS.Utils;

namespace SSCMS.Advertisement.Controllers.Admin
{
    public partial class AddController
    {
        [HttpGet, Route(Route)]
        public async Task<ActionResult<GetResult>> Get([FromQuery] GetRequest request)
        {
            if (!await _authManager.HasSitePermissionsAsync(request.SiteId, AdvertisementUtils.PermissionsAdd))
            {
                return Unauthorized();
            }

            var advertisement = request.AdvertisementId > 0
                ? await _advertisementRepository.GetAsync(request.SiteId, request.AdvertisementId)
                : new Models.Advertisement
                {
                    AdvertisementType = AdvertisementType.FloatImage,
                    StartDate = DateTime.Now,
                    EndDate = DateTime.Now.AddMonths(1),
                    RollingType = RollingType.FollowingScreen,
                    PositionType = PositionType.LeftTop,
                    PositionX = 10,
                    PositionY = 120,
                    IsCloseable = true
                };

            var advertisementTypes = ListUtils.GetSelects<AdvertisementType>();
            var scopeTypes = ListUtils.GetSelects<ScopeType>();

            var site = await _siteRepository.GetAsync(request.SiteId);
            if (site == null) return NotFound();

            var channel = await _channelRepository.GetAsync(request.SiteId);
            var cascade = await _channelRepository.GetCascadeAsync(site, channel, async summary =>
            {
                var count = await _contentRepository.GetCountAsync(site, summary);
                return new
                {
                    Count = count
                };
            });

            var templates = await _templateRepository.GetSummariesAsync(request.SiteId);

            var positionTypes = ListUtils.GetSelects<PositionType>();

            var rollingTypes = ListUtils.GetSelects<RollingType>();

            return new GetResult
            {
                Advertisement = advertisement,
                AdvertisementTypes = advertisementTypes,
                ScopeTypes = scopeTypes,
                Channels = cascade,
                Templates = templates,
                PositionTypes = positionTypes,
                RollingTypes = rollingTypes
            };
        }
    }
}
