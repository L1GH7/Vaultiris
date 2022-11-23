{*
* 2020 Labelgrup
*
* NOTICE OF LICENSE
*
* READ ATTACHED LICENSE.TXT
*
*  @author    Manel Alonso <malonso@labelgrup.com>
*  @copyright 2020 Labelgrup
*  @license   README.TXT
*}

{* This should be hooked at BackOfficeHeader, but it's just a patch module :) *}
<style>
#result {
    background-color: #000;
    border: 1px solid #000;
    color: #FFF;
    padding: 8px;
    font-family: courier new;
}
</style>

<script>
$(document).ready(function () {
	// Load patches Info
	var query = $.ajax({
		type: "POST",
		url: "..{$module_dir|escape:'html':'UTF-8'}ajax.php",
		data: "action=list_patches&token={$sectoken|escape:'html':'UTF-8'}",
		dataType: 'json',
		success: function(res) {
			if (parseInt(res.patches) > 0)
			{
				$('#patch_cve').prop('disabled', '');
				$('#result').append("Found "+res.patches+" patch/es to be applied:\n");
				$.each(res.names, function(i, item) {
					$('#result').append("  + "+item+" ("+res.path[i]+")\n");
				});
			}
			else
			{
				$('#result').append("Your PrestaShop seems to be already patched.");
			}
			doScroll($('#result'));
		}
	});	

	// On click PATCH button
	$("#patch_cve").click(function () {
		var query = $.ajax({
			type: "POST",
			url: "..{$module_dir|escape:'html':'UTF-8'}ajax.php",
			data: "action=patch&token={$sectoken|escape:'html':'UTF-8'}",
			dataType: 'json',
			success: function(res) {
				$('#result').append(res.message);
				doScroll($('#result'));
			}
		});	
	});	
});

function doScroll(element_scr)
{
	bottom = element_scr.prop('scrollHeight') - element_scr.height();
	element_scr.scrollTop(bottom);
}
</script>
{* End of comment *}

<div class="container-fluid">
	<div class="row">
		<div class="col-md-12">		
			<div class="tabbable" id="tabs-780422">
				<ul class="nav nav-tabs">
					<li class="active">
						<a href="#panel-info" data-toggle="tab"><i class="icon icon-info-circle"></i> {l s='Information' mod='lblfixer_cve2020_5250'}</a>
					</li>
				</ul>
				
				<!-- INFORMATION AND CONTROLS -->
				<div class="tab-content">
					<div class="tab-pane active" id="panel-info">
						<div class="panel">
							<br/>
							<h3>{l s='LabelGrup CVE2020-5250 Patcher' mod='lblfixer_cve2020_5250'} {$module_version|escape:'html':'UTF-8'}</h3>
							<p>
								<p style="align:center;">
									<img src="{$module_dir|escape:'htmlall':'UTF-8'}logo.png" alt="LabelGrup CVE2020-5250 Patcher">
								</p>
								<strong>{l s='Thank you for choosing our modules!' mod='lblfixer_cve2020_5250'}</strong><br />
								<p>
									{l s='This module can\'t be distributed or modified without consent of the author in any way.' mod='lblfixer_cve2020_5250'}<br />
								</p>
							</p>
							<div class="panel-footer">
								<a class="btn btn-default" href="mailto:soporte@labelgrup.com" target="_blank">
									<i class="icon-envelope"></i> {l s='Contact with us' mod='lblfixer_cve2020_5250'}
								</a>
								<a class="btn btn-default" href="https://www.labelgrup.com" target="_blank">
									<i class="icon-eye-open"></i> {l s='View our modules' mod='lblfixer_cve2020_5250'}
								</a>
							</div>
						</div>

						<div class="panel">
							<h3><span class="icon-info-circle"></span> {l s='Patch Status:' mod='lblfixer_cve2020_5250'}</h3>
							<div class="controls">
								<textarea readonly id="result" name="result" rows="15">{$motd|escape:'html':'UTF-8'}</textarea>
							</div>
							<div class="row">
								<br/>
								<div class="col-md-12">
									<button type="button" disabled="disabled" id="patch_cve" name="patch_cve" class="btn pull-righ btn-primary col-md-12">
										{l s='Patch now' mod='lblfixer_cve2020_5250'}&nbsp;<i class="icon-bug"></i>
									</button>
								</div>
							</div>
						</div>
					</div>
				</div>
			</div>
		</div>
	</div>
</div>