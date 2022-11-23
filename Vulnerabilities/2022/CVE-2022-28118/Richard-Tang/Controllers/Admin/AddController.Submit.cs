using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;
using SSCMS.Advertisement.Utils;
using SSCMS.Dto;
using SSCMS.Utils;

namespace SSCMS.Advertisement.Controllers.Admin
{
    public partial class AddController
    {
        [HttpPost, Route(Route)]
        public async Task<ActionResult<BoolResult>> Submit([FromBody] SubmitRequest request)
        {
            if (!await _authManager.HasSitePermissionsAsync(request.SiteId, AdvertisementUtils.PermissionsAdd))
            {
                return Unauthorized();
            }

            Models.Advertisement advertisement;
            if (request.AdvertisementId > 0)
            {
                advertisement = await _advertisementRepository.GetAsync(request.SiteId, request.AdvertisementId);
            }
            else
            {
                if (await _advertisementRepository.IsExistsAsync(request.AdvertisementName, request.SiteId))
                {
                    return this.Error("保存失败，已存在相同名称的广告！");
                }

                advertisement = new Models.Advertisement();
            }

            advertisement.SiteId = request.SiteId;
            advertisement.AdvertisementName = request.AdvertisementName;
            advertisement.AdvertisementType = request.AdvertisementType;
            advertisement.ScopeType = request.ScopeType;
            advertisement.ChannelIds = request.ChannelIds;
            advertisement.IsChannels = request.IsChannels;
            advertisement.IsContents = request.IsContents;
            advertisement.TemplateIds = request.TemplateIds;
            advertisement.IsDateLimited = request.IsDateLimited;
            advertisement.StartDate = request.StartDate;
            advertisement.EndDate = request.EndDate;
            advertisement.NavigationUrl = request.NavigationUrl;
            advertisement.ImageUrl = request.ImageUrl;
            advertisement.Width = request.Width;
            advertisement.Height = request.Height;
            advertisement.RollingType = request.RollingType;
            advertisement.PositionType = request.PositionType;
            advertisement.PositionX = request.PositionX;
            advertisement.PositionY = request.PositionY;
            advertisement.IsCloseable = request.IsCloseable;
            advertisement.Delay = request.Delay;

            if (advertisement.Id > 0)
            {
                await _advertisementRepository.UpdateAsync(advertisement);
            }
            else
            {
                await _advertisementRepository.InsertAsync(advertisement);
            }

            return new BoolResult
            {
                Value = true
            };
        }
    }
}
