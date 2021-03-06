var WebKit = require('../');
var expect = require('expect.js');
var fs = require('fs');

describe("destroy method", function suite() {
	it("should be idempotent", function(done) {
		var view = new WebKit();
		view.init(function(err) {
			expect(err).to.not.be.ok();
			view.destroy(function(err) {
				expect(err).to.not.be.ok();
				view.destroy(done);
			});
		});
	});
});
