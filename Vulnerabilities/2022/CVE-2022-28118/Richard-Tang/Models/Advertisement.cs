using System;
using System.Collections.Generic;
using Datory;
using Datory.Annotations;

namespace SSCMS.Advertisement.Models
{
    [DataTable("sscms_advertisement")]
    public class Advertisement : Entity
    {
        [DataColumn]
        public string AdvertisementName { get; set; }

        [DataColumn]
        public int SiteId { get; set; }

        [DataColumn]
        public AdvertisementType AdvertisementType { get; set; }

        [DataColumn]
        public bool IsDateLimited { get; set; }

        [DataColumn]
        public DateTime? StartDate { get; set; }

        [DataColumn]
        public DateTime? EndDate { get; set; }

        public ScopeType ScopeType { get; set; }

        public List<int> ChannelIds { get; set; }

        public bool IsChannels { get; set; }

        public bool IsContents { get; set; }
        
        public List<int> TemplateIds { get; set; }

        public bool IsCloseable { get; set; }

        public PositionType PositionType { get; set; }

        public int PositionX { get; set; }

        public int PositionY { get; set; }

        public RollingType RollingType { get; set; }

        public string NavigationUrl { get; set; }

        public string ImageUrl { get; set; }

        public int Height { get; set; }

        public int Width { get; set; }

        public int Delay { get; set; }
    }
}
