var $url = '/advertisement/add';

var $typeBase = 'Base';
var $typeFloatImage = 'FloatImage';
var $typeScreenDown = 'ScreenDown';
var $typeOpenWindow = 'OpenWindow';
var $typeDone = 'Done';

var data = utils.init({
  siteId: utils.getQueryInt('siteId'),
  advertisementId: utils.getQueryInt('advertisementId'),
  urlUpload: null,
  pageType: $typeBase,
  imageType: 'upload',
  advertisement: null,
  advertisementTypes: null,
  scopeTypes: null,
  channels: null,
  expandedChannelIds: [],
  defaultCheckedIds: [],
  templates: null,
  positionTypes: null,
  rollingTypes: null
});

var methods = {
  apiGet: function () {
    var $this = this;

    $api.get($url, {
      params: {
        siteId: $this.siteId,
        advertisementId: $this.advertisementId
      }
    }).then(function (response) {
      var res = response.data;

      $this.advertisement = res.advertisement;
      if (!$this.advertisement.templateIds) {
        $this.advertisement.templateIds = [];
      }
      $this.advertisementTypes = res.advertisementTypes;
      $this.scopeTypes = res.scopeTypes;
      $this.channels = [res.channels];
      $this.templates = res.templates;
      $this.positionTypes = res.positionTypes;
      $this.rollingTypes = res.rollingTypes;

      $this.defaultCheckedIds = res.advertisement.channelIds;
    }).catch(function (error) {
      utils.error(error);
    }).then(function () {
      $this.pageLoad = true;
    });
  },

  apiSubmit: function () {
    var $this = this;

    utils.loading(this, true);
    $api.post($url, {
      siteId: this.siteId,
      advertisementId: this.advertisementId,
      advertisementName: this.advertisement.advertisementName,
      advertisementType: this.advertisement.advertisementType,
      scopeType: this.advertisement.scopeType,
      channelIds: this.advertisement.channelIds,
      isChannels: this.advertisement.isChannels,
      isContents: this.advertisement.isContents,
      templateIds: this.advertisement.templateIds,
      isDateLimited: this.advertisement.isDateLimited,
      startDate: this.advertisement.startDate,
      endDate: this.advertisement.endDate,
      navigationUrl: this.advertisement.navigationUrl,
      imageUrl: this.advertisement.imageUrl,
      width: this.advertisement.width,
      height: this.advertisement.height,
      rollingType: this.advertisement.rollingType,
      positionType: this.advertisement.positionType,
      positionX: this.advertisement.positionX,
      positionY: this.advertisement.positionY,
      isCloseable: this.advertisement.isCloseable,
      delay: this.advertisement.delay
    }).then(function (response) {
      var res = response.data;

      $this.pageType = $typeDone;

      setTimeout(function() {
        location.href = utils.getPageUrl('advertisement', 'list', {
          siteId: $this.siteId
        });
      }, 2000);
    }).catch(function (error) {
      utils.error(error);
    }).then(function () {
      utils.loading($this, false);
    });
  },

  uploadBefore(file) {
    var re = /(\.jpg|\.jpeg|\.bmp|\.gif|\.png|\.webp)$/i;
    if(!re.exec(file.name))
    {
      utils.error('文件只能是图片格式，请选择有效的文件上传!');
      return false;
    }

    var isLt10M = file.size / 1024 / 1024 < 10;
    if (!isLt10M) {
      utils.error('上传图片大小不能超过 10MB!');
      return false;
    }
    return true;
  },

  uploadProgress: function() {
    utils.loading(this, true);
  },

  uploadSuccess: function(res, file) {
    utils.loading(this, false);
    this.advertisement.imageUrl = res.imageUrl;
    this.advertisement.width = res.width;
    this.advertisement.height = res.height;
  },

  uploadError: function(err) {
    utils.loading(this, false);
    var error = JSON.parse(err.message);
    utils.error(error.message);
  },

  btnPreviousClick: function () {
    this.pageType = $typeBase;
  },

  btnSubmitClick: function () {
    var $this = this;
    this.$refs.form.validate(function(valid) {
      if (valid) {
        if ($this.pageType === $typeBase) {
          $this.pageType = $this.advertisement.advertisementType;
        } else {
          $this.apiSubmit();
        }
      }
    });
  },

  handleCheckChange() {
    this.advertisement.channelIds = this.$refs.tree.getCheckedKeys();
  },

  getChannelUrl: function(data) {
    return utils.getRootUrl('redirect', {
      siteId: this.siteId,
      channelId: data.value
    });
  },

  getTemplateType: function(templateType) {
    if (templateType === 'IndexPageTemplate') {
      return '首页模板';
    } else if (templateType === 'ChannelTemplate') {
      return '栏目模板';
    } else if (templateType === 'ContentTemplate') {
      return '内容模板';
    } else if (templateType === 'FileTemplate') {
      return '单页模板';
    }
    return '';
  }
};

var vue = new Vue({
  el: '#main',
  data: data,
  methods: methods,
  created: function () {
    this.apiGet();
    this.urlUpload = $apiUrl + $url + '/actions/upload?siteId=' + this.siteId;
    this.expandedChannelIds = [this.siteId];
  }
});
