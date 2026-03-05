#include "Frame.h"
#include "Test.h"

int FuncDoodle_RunFrameTests() {
	TEST_SCOPE("Frame Tests");

	FuncDoodle::Frame frame1;
	CHECK((frame1.Width() == 1), "Default width should be 1");
	CHECK((frame1.Height() == 1), "Default height should be 1");

	FuncDoodle::Frame frame2(10, 20, FuncDoodle::Col{255, 0, 0});
	CHECK((frame2.Width() == 10), "width should be 10");
	CHECK((frame2.Height() == 20), "height should be 20");
	CHECK((frame2.Pixels()->BgCol() == FuncDoodle::Col{255, 0, 0}),
		"bgCol should be red");

	FuncDoodle::Frame frame3(5, 5, FuncDoodle::Col{0, 255, 0});
	frame3.SetPixel(0, 0, FuncDoodle::Col{255, 0, 0});
	frame3.SetPixel(4, 4, FuncDoodle::Col{0, 0, 255});
	CHECK((frame3.Pixels()->Get(0, 0) == FuncDoodle::Col{255, 0, 0}),
		"SetPixel should work at (0,0)");
	CHECK((frame3.Pixels()->Get(4, 4) == FuncDoodle::Col{0, 0, 255}),
		"SetPixel should work at (4,4)");

	FuncDoodle::Col center_col = frame3.Pixels()->Get(2, 2);
	CHECK((center_col == FuncDoodle::Col{0, 255, 0}),
		"Unset pixel should have bgCol");

	FuncDoodle::Frame frame_copy = frame2;
	CHECK((frame_copy.Width() == frame2.Width()),
		"Copy constructor should preserve width");
	CHECK((frame_copy.Height() == frame2.Height()),
		"Copy constructor should preserve height");

	FuncDoodle::Frame frame_assigned;
	frame_assigned = frame2;
	CHECK((frame_assigned.Width() == frame2.Width()),
		"Assignment should preserve width");
	CHECK((frame_assigned.Height() == frame2.Height()),
		"Assignment should preserve height");

	FuncDoodle::Frame frame_eq1(10, 10, FuncDoodle::Col{100, 100, 100});
	FuncDoodle::Frame frame_eq2(10, 10, FuncDoodle::Col{100, 100, 100});
	CHECK((frame_eq1 == frame_eq2), "Identical frames should be equal");

	FuncDoodle::Frame frame_neq(10, 10, FuncDoodle::Col{100, 100, 101});
	CHECK((frame_eq1 != frame_neq), "Different frames should not be equal");

	FuncDoodle::Frame frame_diff_size1(10, 10, FuncDoodle::Col{100, 100, 100});
	FuncDoodle::Frame frame_diff_size2(20, 10, FuncDoodle::Col{100, 100, 100});
	CHECK((frame_diff_size1 != frame_diff_size2),
		"Different size frames should not be equal");

	FuncDoodle::Frame frame_reinit(5, 5, FuncDoodle::Col{0, 0, 0});
	frame_reinit.ReInit(100, 50, FuncDoodle::Col{128, 128, 128});
	CHECK((frame_reinit.Width() == 100), "ReInit should change width");
	CHECK((frame_reinit.Height() == 50), "ReInit should change height");
	CHECK((frame_reinit.Pixels()->BgCol() == FuncDoodle::Col{128, 128, 128}),
		"ReInit should change bgCol");

	std::vector<FuncDoodle::Col> data = frame2.Data();
	CHECK((data.size() == 200), "Data size should be width * height");

	FuncDoodle::ImageArray arr(3, 3, FuncDoodle::Col{255, 255, 0});
	FuncDoodle::Frame frame_from_arr(arr);
	CHECK((frame_from_arr.Width() == 3),
		"Frame from ImageArray width should be 3");
	CHECK((frame_from_arr.Height() == 3),
		"Frame from ImageArray height should be 3");

	FuncDoodle::ImageArray* arr_ptr =
		new FuncDoodle::ImageArray(4, 4, FuncDoodle::Col{0, 255, 255});
	FuncDoodle::Frame frame_from_ptr(arr_ptr);
	CHECK(
		(frame_from_ptr.Width() == 4), "Frame from pointer width should be 4");
	CHECK((frame_from_ptr.Height() == 4),
		"Frame from pointer height should be 4");
	delete arr_ptr;

	FuncDoodle::ImageArray* null_arr = nullptr;
	FuncDoodle::Frame frame_from_null_ptr(null_arr);
	CHECK((frame_from_null_ptr.Width() == 1),
		"Frame from null ptr should have default width");
	CHECK((frame_from_null_ptr.Height() == 1),
		"Frame from null ptr should have default height");

	FuncDoodle::Frame frame_mut = frame2;
	frame_mut.PixelsMut()->SetBG(FuncDoodle::Col{255, 255, 255});
	CHECK((frame_mut.Pixels()->BgCol() == FuncDoodle::Col{255, 255, 255}),
		"PixelsMut should allow mutation");

	return 0;
}
