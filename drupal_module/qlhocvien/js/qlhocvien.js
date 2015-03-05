(function ($) {
   
})(jQuery);

function capnhatnganh(khoa) {
    var khoanganh = [];
    khoanganh["CNPM"] = "D480103";
    khoanganh["HTTT"] = "D480104";
    khoanganh["KHMT"] = "D480101";
    khoanganh["KTMT"] = "D520214";
    khoanganh["MMT&TT"] = "D480102";
    document.getElementsByName('chuyennganh')[0].value = khoanganh[khoa];
}

