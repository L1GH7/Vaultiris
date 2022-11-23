using System;
using SSCMS.Enums;
using SSCMS.Parse;
using SSCMS.Utils;
using ScopeType = SSCMS.Advertisement.Models.ScopeType;

namespace SSCMS.Advertisement.Utils
{
    public static class AdvertisementUtils
    {
        public const string PluginId = "sscms.advertisement";
        public const string PermissionsAdd = "advertisement_add";
        public const string PermissionsList = "advertisement_list";

        public const string AssetsUrlClose = "/assets/advertisement/close.gif";
        public const string AssetsUrlAdFloating = "/assets/advertisement/adFloating.js";
        public const string AssetsUrlJquery = "/assets/advertisement/jquery-1.9.1.min.js";

        public static bool IsAdvertisement(IParseContext context, Models.Advertisement advertisement)
        {
            if (advertisement.IsDateLimited)
            {
                if (DateTime.Now < advertisement.StartDate || DateTime.Now > advertisement.EndDate)
                {
                    return false;
                }
            }

            if (advertisement.ScopeType == ScopeType.All)
            {
                return true;
            }

            if (advertisement.ScopeType == ScopeType.Templates)
            {
                return ListUtils.Contains(advertisement.TemplateIds, context.TemplateId);
            }

            if (advertisement.ScopeType == ScopeType.Channels)
            {
                if (context.TemplateType == TemplateType.FileTemplate) return false;
                if (!advertisement.IsChannels && (context.TemplateType == TemplateType.ContentTemplate || context.TemplateType == TemplateType.FileTemplate)) return false;
                if (!advertisement.IsContents && context.TemplateType == TemplateType.ContentTemplate) return false;

                return ListUtils.Contains(advertisement.ChannelIds, context.ChannelId);
            }

            return false;
        }
    }
}
