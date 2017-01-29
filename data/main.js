$(function () {
    /*
    FUNCTIONS
    */
    function updateList() {
        console.log(filters);
        $("#mirrors tr").each(function() {
            var country = $("td:eq(1)", this).text();
            if(country) {
                if(filters["country"] == "all")
                    country = "all";
                var protocol = $("td:eq(2)", this).text();
                if(protocol) {
                    condition = filters["country"] == country && filters[protocol];
                    $(this).toggle(condition);
                }
            }
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
        updateList();
    });
    $('input[type="checkbox"]').change(function() {
        var filter = $(this).parent().find(".custom-control-description").text().toLowerCase();
        filters[filter] = $(this).is(":checked");
        updateList();
    });
});
