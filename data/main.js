$(function () {
    /*
    FUNCTIONS
    */
    function applyCountry() {
        $("#mirrors tr").each(function() {
            var country = $("td:eq(1)", this).text();
            if(country) {
                if(filters["country"] != "all") {
                    filters["country"] != country ? $(this).hide() : $(this).show();
                }
                else
                    $(this).show();
            }
        });
    }
    function applyProtocol() {
        $("#mirrors tr").each(function() {
            var protocol = $("td:eq(2)", this).text();
            if(protocol)
                !filters[protocol] ? $(this).hide() : $(this).show();
        });
    }

    /*
    MAIN
    */
    // Enable tooltips
    $('[data-toggle="tooltip"]').tooltip()
    // Enable table sorting
    $("#mirrors").tablesorter({
        sortList: [[1, 0]]
    });
    // Filters
    var filters = {
        "country": "all",
        "http": true,
        "https": true,
        "ftp": true
    }

    $("#country-select").change(function() {
        var filter = $(this).find("option:selected").text();
        filters["country"] = (filter == "All countries") ? "all" : filter;
        applyCountry();
    });
    $('input[type="checkbox"]').change(function() {
        var filter = $(this).parent().find(".custom-control-description").text().toLowerCase();
        filters[filter] = $(this).is(":checked");
        applyProtocol();
    });
});
