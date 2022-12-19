describe JSON do

  MARSHALLED_OBJECT = <<-JSON
    {"json_class":"JSON::GenericObject","foo":"bar"}
  JSON

  before :each do
    JSON::GenericObject.json_creatable = true
  end

  describe 'JSON.load(string)' do

    it 'does unmarshal' do
      result = JSON.load(MARSHALLED_OBJECT)
      expect(result).to be_a(JSON::GenericObject)
    end

  end

  describe 'JSON(string)' do

    it 'does not unmarshal' do
      result = JSON(MARSHALLED_OBJECT, nil)
      expect(result).to be_a(Hash)
    end

  end

  describe 'JSON.parse(string)' do

    it 'does not unmarshal' do
      result = JSON.parse(MARSHALLED_OBJECT)
      expect(result).to be_a(Hash)
    end

  end

  describe 'JSON.parse(string, nil)' do

    it 'does not unmarshal' do
      result = JSON.parse(MARSHALLED_OBJECT, nil)
      expect(result).to be_a(Hash)
    end

  end

  describe 'JSON.parse(string, create_additions: true)' do

    it 'does unmarshal' do
      result = JSON.parse(MARSHALLED_OBJECT, { :create_additions => true })
      expect(result).to be_a(JSON::GenericObject)
    end

  end

end
