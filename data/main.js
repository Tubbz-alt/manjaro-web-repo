$(function () {
    /*
    FUNCTIONS
    */
    function updateList() {
        // Update list of mirrors
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
        "http": true,
        "https": true,
        "ftp": true
    }
    $('input[type="checkbox"]').change(function() {
        var filter = $(this).parent().find(".custom-control-description").text().toLowerCase();
        filters[filter] = $(this).is(":checked");
        updateList();
    });
});
