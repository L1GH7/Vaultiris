var $url = '/advertisement/list';
var $urlDelete = $url + '/actions/delete';

var data = utils.init({
  siteId: utils.getQueryInt('siteId'),
  advertisementType: utils.getQueryString('advertisementType'),
  advertisements: null,
  types: null,
});

var methods = {
  apiGetList: function () {
    var $this = this;

    utils.loading(this, true);
    $api.get($url, {
      params: {
        siteId: $this.siteId,
        advertisementType: $this.advertisementType
      }
    }).then(function (response) {
      var res = response.data;

      $this.advertisements = res.advertisements;
      $this.types = res.types;
    }).catch(function (error) {
      utils.error(error);
    }).then(function () {
      utils.loading($this, false);
    });
  },

  apiDelete: function (item) {
    var $this = this;

    utils.loading(this, true);
    $api.post($urlDelete, {
      siteId: $this.siteId,
      advertisementId: item.id,
      advertisementType: $this.advertisementType
    }).then(function (response) {
      var res = response.data;

      $this.advertisements = res.advertisements;
      utils.success('广告删除成功');
    }).catch(function (error) {
      utils.error(error);
    }).then(function () {
      utils.loading($this, false);
    });
  },

  btnAddClick: function () {
    location.href = utils.getPageUrl('advertisement', 'add', {
      siteId: this.siteId
    });
  },

  btnEditClick: function (item) {
    location.href = utils.getPageUrl('advertisement', 'add', {
      siteId: this.siteId,
      advertisementId: item.id
    });
  },

  btnDeleteClick: function (item) {
    var $this = this;

    utils.alertDelete({
      title: '删除漂浮广告',
      text: '此操作将删除漂浮广告' + item.advertisementName + '，确定吗？',
      callback: function () {
        $this.apiDelete(item);
      }
    });
  }
};

var $vue = new Vue({
  el: '#main',
  data: data,
  watch: {
    advertisementType: function (val, oldVal) {
      this.apiGetList();
    }
  },
  methods: methods,
  created: function () {
    this.apiGetList();
  }
});
