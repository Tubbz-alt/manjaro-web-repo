$(function () {
    // Javascript is enabled
    $("body").removeClass("no-js");
    /*
    FUNCTIONS
    */
    function updateList() {
        $("#mirrors tr").each(function() {
            var country = $("td:eq(1)", this).text();
            country = country.split(" ").join("_");
            if(country) {
                if(filters["country"] == "all")
                    country = "all";
                var protocols = $("td:eq(2)", this).text();
                if(protocols) {
                    condition = filters["country"] == country;
                    protocols = protocols.split(", ");
                    if(filters["http"] && protocols.includes("http")) {
                        condition = condition && true;
                    }
                    else if(filters["https"] && protocols.includes("https")) {
                        condition = condition && true;
                    }
                    else if(filters["ftp"] && protocols.includes("ftp")) {
                        condition = condition && true;
                    }
                    else {
                        condition = condition && false;
                    }
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
    $.tablesorter.addParser({
        id: "branch",
        is: function(s, table, cell, $cell) {
            return false;
        },
        format: function(s, table, cell, cellIndex) {
            if($(cell).html().includes("up"))
                return 0
            else if($(cell).html().includes("out"))
                return 1
            else
                return 2
        },
        type: "numeric"
    });
    $.tablesorter.addParser({
        id: "sync",
        is: function(s, table, cell, $cell) {
            return false;
        },
        format: function(s, table, cell, cellIndex) {
            nb = $(cell).text().split(":");
            return +(nb[0] + "." + nb[1]);
        },
        type: "numeric"
    });
    $("#mirrors").tablesorter();
    // Filters
    var filters = {
        "country": "all",
        "http": true,
        "https": true,
        "ftp": true
    }

    $("#country-select").change(function() {
        var country = $(this).find("option:selected").text();
        country = country.split(" ").join("_");
        filters["country"] = (country == "All_countries") ? "all" : country;
        updateList();
    });
    $('input[type="checkbox"]').change(function() {
        var protocol = $(this).parent().find(".custom-control-description").text().toLowerCase();
        filters[protocol] = $(this).is(":checked");
        updateList();
    });
});
