$(function () {
    // Enable tooltips
    $('[data-toggle="tooltip"]').tooltip()
    // Enable table sorting
    $('#mirrors').tablesorter({
        sortList: [[1, 0]]
    });
});
