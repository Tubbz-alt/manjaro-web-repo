$(function () {
    // Javascript is enabled
    $("body").removeClass("no-js");
    /*
    FUNCTIONS
    */
    function updateList() {
        // Update list with filters
        $("#mirrors tr").each(function() {
            var country = $("td:eq(1)", this).text();
            country = country.split(" ").join("_").toLowerCase();
            if(country) {
                if(filters["country"] == "all")
                    country = "all";
                var protocols = $("td:eq(2)", this).text();
                if(protocols) {
                    protocols = protocols.split(", ");
                    if(filters["http"] && protocols.includes("http")) {
                        condition = true;
                    }
                    else if(filters["https"] && protocols.includes("https")) {
                        condition = true;
                    }
                    else if(filters["ftp"] && protocols.includes("ftp")) {
                        condition = true;
                    }
                    else {
                        condition = false;
                    }
                    condition = condition && (filters["country"] == country);
                    console.log(filters["country"]);
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
        "country": $("#country-filter").val(),
        "http": $("#http-filter").is(":checked"),
        "https": $("#https-filter").is(":checked"),
        "ftp": $("#ftp-filter").is(":checked")
    }
    // Refresh list at start
    updateList();
    // Events on inputs
    $("#country-filter").change(function() {
        filters["country"] = $(this).val();
        updateList();
    });
    $('input[type="checkbox"]').change(function() {
        var protocol = $(this).attr("id").split("-")[0];
        filters[protocol] = $(this).is(":checked");
        updateList();
    });
});
