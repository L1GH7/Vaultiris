using Microsoft.AspNetCore.Mvc;

namespace SSCMS.Advertisement.Controllers
{
    [Route("api/advertisement/ping")]
    public class PingController : ControllerBase
    {
        private const string Route = "";

        [HttpGet, Route(Route)]
        public string Get()
        {
            return "pong";
        }
    }
}
