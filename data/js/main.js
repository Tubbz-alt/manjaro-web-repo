$(function () {
    window.onload = function() {
        location.reload(true);
    };
    // Javascript is enabled
    document.body.classList.remove("no-js");

    // Enable table sorting
    $.tablesorter.addParser({
        id: "branch",
        is: function (s, table, cell, $cell) {
            return false;
        },
        format: function (s, table, cell, cellIndex) {
            if ($(cell).html().includes("up")) {
                return 1;
            } else if ($(cell).html().includes("out")) {
                return 0;
            } else {
                return -1;
            }
        },
        type: "numeric"
    });
    $.tablesorter.addParser({
        id: "sync",
        is: function (s, table, cell, $cell) {
            return false;
        },
        format: function (s, table, cell, cellIndex) {
            nb = $(cell).text().split(":");
            return +(nb[0] + "." + nb[1]);
        },
        type: "numeric"
    });
    $("#mirrors").tablesorter({sortList: [[1, 0]]});

    // Enable tooltips
    $("[data-toggle=\"tooltip\"]").tooltip();

    function updateList(filters) {
        // Update list with filters
        var table = document.getElementById("mirrors");
        // Iterate table rows
        Object.keys(table.rows).forEach(function (key) {
            if (key == 0) {
                return;
            }
            var row = table.rows[key];
            var country = row.cells[1].textContent;
            if (filters.country === "all") {
                country = "all";
            } else {
                country = country.split(" ").join("_").toLowerCase();
            }
            if (country) {
                var protocols = row.cells[2].textContent;
                if (protocols) {
                    protocols = protocols.split(", ");
                    // Check filters
                    var condition = false;
                    if (filters.http && protocols.includes("http")) {
                        condition = true;
                    } else if (filters.https && protocols.includes("https")) {
                        condition = true;
                    } else if (filters.ftp && protocols.includes("ftp")) {
                        condition = true;
                    }
                    condition = condition && (filters.country === country);
                    if (condition) {
                        row.style.display = "table-row";
                    } else {
                        row.style.display = "none";
                    }
                }
            }
        });
    }

    // Filters elts
    var country_f = document.getElementById("country-filter");

    // Filters
    var filters = {
        "http": document.getElementById("http-filter").checked,
        "https": document.getElementById("https-filter").checked,
        "ftp": document.getElementById("ftp-filter").checked,
        "country": country_f.value
    };

    // Refresh list at start
    updateList(filters);

    // Events on inputs
    var checkboxes = document.querySelectorAll(".custom-control-input");
    Object.keys(checkboxes).forEach(function (key) {
        checkboxes[key].addEventListener("change", function () {
            var protocol = checkboxes[key].id.split("-")[0];
            filters[protocol] = checkboxes[key].checked;
            updateList(filters);
        });
    });
    country_f.addEventListener("change", function () {
        filters.country = country_f.value;
        updateList(filters);
    });
});
