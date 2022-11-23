using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Datory;
using SSCMS.Advertisement.Abstractions;
using SSCMS.Advertisement.Models;
using SSCMS.Advertisement.Utils;
using SSCMS.Parse;
using SSCMS.Services;
using SSCMS.Repositories;
using SSCMS.Utils;

namespace SSCMS.Advertisement.Core
{
    public class AdvertisementRepository : IAdvertisementRepository
    {
        private readonly IPathManager _pathManager;
        private readonly ISiteRepository _siteRepository;
        private readonly Repository<Models.Advertisement> _repository;

        public AdvertisementRepository(IPathManager pathManager, ISettingsManager settingsManager, ISiteRepository siteRepository)
        {
            _pathManager = pathManager;
            _siteRepository = siteRepository;
            _repository = new Repository<Models.Advertisement>(new Database(settingsManager.DatabaseType, settingsManager.DatabaseConnectionString), settingsManager.Redis);
        }

        private static string GetCacheKey(int siteId)
        {
            return $"SSCMS.Advertisement:{siteId}";
        }

        public async Task<Models.Advertisement> GetAsync(int siteId, int advertisementId)
        {
            var advertisements = await GetAllAsync(siteId);
            return advertisements.FirstOrDefault(x => x.Id == advertisementId);
        }

        public async Task<int> InsertAsync(Models.Advertisement ad)
        {
            return await _repository.InsertAsync(ad, Q.CachingRemove(GetCacheKey(ad.SiteId)));
        }

        public async Task<bool> UpdateAsync(Models.Advertisement ad)
        {
            return await _repository.UpdateAsync(ad, Q.CachingRemove(GetCacheKey(ad.SiteId)));
        }

        public async Task DeleteAsync(int siteId, int advertisementId)
        {
            await _repository.DeleteAsync(advertisementId, Q.CachingRemove(GetCacheKey(siteId)));
        }

        public async Task<bool> IsExistsAsync(string advertisementName, int siteId)
        {
            var advertisements = await GetAllAsync(siteId);
            return advertisements.Exists(x => x.AdvertisementName == advertisementName);
        }

        public async Task<List<Models.Advertisement>> GetAllAsync(int siteId)
        {
            return await _repository.GetAllAsync(Q
                .Where(nameof(Models.Advertisement.SiteId), siteId)
                .OrderByDesc(nameof(Models.Advertisement.Id))
                .CachingGet(GetCacheKey(siteId))
            );
        }

        public async Task<List<Models.Advertisement>> GetAllAsync(int siteId, AdvertisementType advertisementType)
        {
            var advertisements = await GetAllAsync(siteId);
            return advertisements.Where(x => x.AdvertisementType == advertisementType).ToList();
        }

        public async Task AddAdvertisementsAsync(IParseContext context)
        {
            var site = await _siteRepository.GetAsync(context.SiteId);
            var apiUrl = _pathManager.GetApiHostUrl(site);
            var advertisements = await GetAllAsync(context.SiteId);

            foreach (var advertisement in advertisements)
            {
                if (!AdvertisementUtils.IsAdvertisement(context, advertisement)) continue;

                var scripts = string.Empty;
                if (advertisement.AdvertisementType == AdvertisementType.FloatImage)
                {
                    context.HeadCodes[AdvertisementUtils.PluginId] = @$"<script type=""text/javascript"" src=""{PageUtils.Combine(apiUrl, AdvertisementUtils.AssetsUrlAdFloating)}""></script>";

                    var floatScript = new ScriptFloating(_pathManager, site, apiUrl, advertisement);
                    scripts = await floatScript.GetScriptAsync();
                }
                else if (advertisement.AdvertisementType == AdvertisementType.ScreenDown)
                {
                    if (!context.HeadCodes.ContainsKey("Jquery"))
                    {
                        context.HeadCodes[AdvertisementUtils.PluginId] = @$"<script type=""text/javascript"" src=""{PageUtils.Combine(apiUrl, AdvertisementUtils.AssetsUrlJquery)}""></script>";
                    }

                    var screenDownScript = new ScriptScreenDown(_pathManager, site, advertisement);
                    scripts = await screenDownScript.GetScriptAsync();
                }
                else if (advertisement.AdvertisementType == AdvertisementType.OpenWindow)
                {
                    var openWindowScript = new ScriptOpenWindow(advertisement);
                    scripts = openWindowScript.GetScript();
                }

                context.BodyCodes[$"{AdvertisementUtils.PluginId}_{advertisement.Id}"] = scripts;
            }
        }
    }
}
