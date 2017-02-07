$(function () {
    // Javascript is enabled
    document.body.classList.remove("no-js");
    /*
    FUNCTIONS
    */
    function updateList() {
        // Update list with filters
        var table = document.getElementById("mirrors");
        for (var i = 1; i < table.rows.length; i++) {
            var row = table.rows[i];
            var country = table.rows[i].cells[1].textContent;
            if(filters["country"] == "all") {
                country = "all";
            } else {
                country = country.split(" ").join("_").toLowerCase();
            }
            if(country) {
                var protocols = table.rows[i].cells[2].textContent;
                if(protocols) {
                    protocols = protocols.split(", ");
                    if(filters["http"] && protocols.includes("http")) {
                        condition = true;
                    } else if(filters["https"] && protocols.includes("https")) {
                        condition = true;
                    } else if(filters["ftp"] && protocols.includes("ftp")) {
                        condition = true;
                    } else {
                        condition = false;
                    }
                    condition = condition && (filters["country"] == country);
                    if(condition) {
                        row.style.display = "table-row";
                    }
                    else {
                        row.style.display = "none";
                    }
                }
            }
        }
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

    // Filters elts
    var country_f = document.getElementById("country-filter");
    var http_f = document.getElementById("http-filter");
    var https_f = document.getElementById("http-filter");
    var ftp_f = document.getElementById("http-filter");

    // Filters
    var filters = {
        "http": http_f.checked,
        "https": https_f.checked,
        "ftp": ftp_f.checked,
        "country": country_f.value
    }

    // Refresh list at start
    updateList();

    // Events on inputs
    var checkboxes = document.querySelectorAll(".custom-control-input");
    for (var i = 0; i < checkboxes.length; i++) {
        checkboxes[i].addEventListener("change", function() {
            var protocol = this.id.split("-")[0];
            filters[protocol] = this.checked;
            updateList();
        });
    }
    country_f.addEventListener("change", function() {
        filters["country"] = this.value;
        updateList();
    });
});
