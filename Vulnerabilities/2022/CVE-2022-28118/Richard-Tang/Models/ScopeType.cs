using Datory.Annotations;
using Newtonsoft.Json;
using Newtonsoft.Json.Converters;

namespace SSCMS.Advertisement.Models
{
    [JsonConverter(typeof(StringEnumConverter))]
    public enum ScopeType
	{
        [DataEnum(DisplayName = "整站显示")] All,
        [DataEnum(DisplayName = "按栏目显示")] Channels,
        [DataEnum(DisplayName = "按模板显示")] Templates,
    }
}
