using System.Collections.Generic;
using System.Text;
using System.Threading.Tasks;
using Datory;
using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Mvc;
using SSCMS.Advertisement.Abstractions;
using SSCMS.Advertisement.Models;
using SSCMS.Configuration;
using SSCMS.Repositories;
using SSCMS.Services;

namespace SSCMS.Advertisement.Controllers.Admin
{
    [Authorize(Roles = Types.Roles.Administrator)]
    [Route(Constants.ApiAdminPrefix)]
    public partial class ListController : ControllerBase
    {
        private const string Route = "advertisement/list";
        private const string RouteDelete = "advertisement/list/actions/delete";

        private readonly IAuthManager _authManager;
        private readonly IChannelRepository _channelRepository;
        private readonly ITemplateRepository _templateRepository;
        private readonly IAdvertisementRepository _advertisementRepository;
        public ListController(IAuthManager authManager, IChannelRepository channelRepository, ITemplateRepository templateRepository, IAdvertisementRepository advertisementRepository)
        {
            _authManager = authManager;
            _channelRepository = channelRepository;
            _templateRepository = templateRepository;
            _advertisementRepository = advertisementRepository;
        }

        public class GetRequest
        {
            public int SiteId { get; set; }
            public string AdvertisementType { get; set; }
        }

        public class GetResult
        {
            public List<Models.Advertisement> Advertisements { get; set; }
            public List<KeyValuePair<string, string>> Types { get; set; }
        }

        public class DeleteRequest
        {
            public int SiteId { get; set; }
            public int AdvertisementId { get; set; }
            public string AdvertisementType { get; set; }
        }

        public class DeleteResult
        {
            public List<Models.Advertisement> Advertisements { get; set; }
        }

        private async Task<string> GetDisplayAsync(int siteId, Models.Advertisement ad)
        {
            var builder = new StringBuilder();
            if (ad.ScopeType == ScopeType.Channels)
            {
                foreach (var channelId in ad.ChannelIds)
                {
                    var channelName = await _channelRepository.GetChannelNameNavigationAsync(siteId, channelId);
                    if (!string.IsNullOrEmpty(channelName))
                    {
                        builder.Append(channelName);
                    }
                    builder.Append(",");
                }
                builder.Length--;
            }
            else if (ad.ScopeType == ScopeType.Templates)
            {
                if (ad.TemplateIds != null)
                {
                    foreach (var templateId in ad.TemplateIds)
                    {
                        var templateName = await _templateRepository.GetTemplateNameAsync(templateId);
                        if (!string.IsNullOrEmpty(templateName))
                        {
                            builder.Append(templateName);
                        }
                        builder.Append(",");
                    }
                    builder.Length--;
                }
            }

            return builder.Length > 0 ? $"{ad.ScopeType.GetDisplayName()} - {builder}" : ad.ScopeType.GetDisplayName();
        }
    }
}
