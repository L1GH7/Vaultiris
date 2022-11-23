using System.Threading.Tasks;
using SSCMS.Advertisement.Abstractions;
using SSCMS.Parse;
using SSCMS.Plugins;

namespace SSCMS.Advertisement.Core
{
    public class CreateStartAsync : IPluginCreateStartAsync
    {
        private readonly IAdvertisementRepository _advertisementRepository;
        public CreateStartAsync(IAdvertisementRepository advertisementRepository)
        {
            _advertisementRepository = advertisementRepository;
        }

        public async Task ParseAsync(IParseContext context)
        {
            await _advertisementRepository.AddAdvertisementsAsync(context);
        }
    }
}
