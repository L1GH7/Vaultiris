using System.IO;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Mvc;
using SSCMS.Advertisement.Utils;
using SSCMS.Dto;
using SSCMS.Utils;

namespace SSCMS.Advertisement.Controllers.Admin
{
    public partial class AddController
    {
        [RequestSizeLimit(long.MaxValue)]
        [HttpPost, Route(RouteActionsUpload)]
        public async Task<ActionResult<UploadResult>> Upload([FromQuery] SiteRequest request, [FromForm] IFormFile file)
        {
            if (!await _authManager.HasSitePermissionsAsync(request.SiteId, AdvertisementUtils.PermissionsAdd))
            {
                return Unauthorized();
            }

            var site = await _siteRepository.GetAsync(request.SiteId);

            if (file == null)
            {
                return this.Error("请选择有效的文件上传!");
            }

            var fileName = Path.GetFileName(file.FileName);

            var fileExtName = PathUtils.GetExtension(fileName).ToLower();
            var localDirectoryPath = await _pathManager.GetUploadDirectoryPathAsync(site, fileExtName);
            var localFileName = _pathManager.GetUploadFileName(site, fileName);
            var filePath = PathUtils.Combine(localDirectoryPath, localFileName);

            if (!_pathManager.IsImageExtensionAllowed(site, fileExtName))
            {
                return this.Error("此图片格式已被禁止上传，请转换格式后上传!");
            }
            if (!_pathManager.IsImageSizeAllowed(site, file.Length))
            {
                return this.Error("此图片大小已超过限制，请压缩后上传!");
            }

            await _pathManager.UploadAsync(file, filePath);

            var imageUrl = await _pathManager.GetSiteUrlByPhysicalPathAsync(site, filePath, true);

            var (width, height) = _pathManager.GetImageSize(filePath);

            return new UploadResult
            {
                ImageUrl = imageUrl,
                Width = width,
                Height = height
            };
        }
    }
}
