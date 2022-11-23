using System;
using System.Collections.Generic;
using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Mvc;
using SSCMS.Advertisement.Abstractions;
using SSCMS.Advertisement.Models;
using SSCMS.Configuration;
using SSCMS.Dto;
using SSCMS.Models;
using SSCMS.Repositories;
using SSCMS.Services;

namespace SSCMS.Advertisement.Controllers.Admin
{
    [Authorize(Roles = Types.Roles.Administrator)]
    [Route(Constants.ApiAdminPrefix)]
    public partial class AddController : ControllerBase
    {
        private const string Route = "advertisement/add";
        private const string RouteActionsUpload = "advertisement/add/actions/upload";

        private readonly IAuthManager _authManager;
        private readonly IPathManager _pathManager;
        private readonly ISiteRepository _siteRepository;
        private readonly IChannelRepository _channelRepository;
        private readonly IContentRepository _contentRepository;
        private readonly ITemplateRepository _templateRepository;
        private readonly IAdvertisementRepository _advertisementRepository;
        public AddController(IAuthManager authManager, IPathManager pathManager, ISiteRepository siteRepository, IChannelRepository channelRepository, IContentRepository contentRepository, ITemplateRepository templateRepository, IAdvertisementRepository advertisementRepository)
        {
            _authManager = authManager;
            _pathManager = pathManager;
            _siteRepository = siteRepository;
            _channelRepository = channelRepository;
            _contentRepository = contentRepository;
            _templateRepository = templateRepository;
            _advertisementRepository = advertisementRepository;
        }

        public class GetRequest
        {
            public int SiteId { get; set; }
            public int AdvertisementId { get; set; }
        }

        public class GetResult
        {
            public Models.Advertisement Advertisement { get; set; }
            public IEnumerable<Select<string>> AdvertisementTypes { get; set; }
            public IEnumerable<Select<string>> ScopeTypes { get; set; }
            public Cascade<int> Channels { get; set; }
            public List<TemplateSummary> Templates { get; set; }
            public IEnumerable<Select<string>> PositionTypes { get; set; }
            public IEnumerable<Select<string>> RollingTypes { get; set; }
        }

        public class UploadResult
        {
            public string ImageUrl { get; set; }
            public int Width { get; set; }
            public int Height { get; set; }
        }

        public class SubmitRequest
        {
            public int SiteId { get; set; }
            public int AdvertisementId { get; set; }
            public string AdvertisementName { get; set; }
            public AdvertisementType AdvertisementType { get; set; }
            public ScopeType ScopeType { get; set; }
            public List<int> ChannelIds { get; set; }
            public bool IsChannels { get; set; }
            public bool IsContents { get; set; }
            public List<int> TemplateIds { get; set; }
            public bool IsDateLimited { get; set; }
            public DateTime StartDate { get; set; }
            public DateTime EndDate { get; set; }
            public string NavigationUrl { get; set; }
            public string ImageUrl { get; set; }
            public int Width { get; set; }
            public int Height { get; set; }
            public RollingType RollingType { get; set; }
            public PositionType PositionType { get; set; }
            public int PositionX { get; set; }
            public int PositionY { get; set; }
            public bool IsCloseable { get; set; }
            public int Delay { get; set; }
        }
    }
}
