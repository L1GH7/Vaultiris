using System.Collections.Generic;
using System.Threading.Tasks;
using SSCMS.Advertisement.Models;
using SSCMS.Parse;

namespace SSCMS.Advertisement.Abstractions
{
    public interface IAdvertisementRepository
    {
        Task<Models.Advertisement> GetAsync(int siteId, int advertisementId);

        Task<int> InsertAsync(Models.Advertisement ad);

        Task<bool> UpdateAsync(Models.Advertisement ad);

        Task DeleteAsync(int siteId, int advertisementId);

        Task<bool> IsExistsAsync(string advertisementName, int siteId);

        Task<List<Models.Advertisement>> GetAllAsync(int siteId);

        Task<List<Models.Advertisement>> GetAllAsync(int siteId, AdvertisementType advertisementType);

        Task AddAdvertisementsAsync(IParseContext context);
    }
}
