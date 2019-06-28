#include <fstream>
#include <iostream>
#include <thread>
#include "window_helpers.hpp"

#include "DepthFrame.hpp"

#include <nimbus/entities/Camera.hpp>
#include <nimbus/entities/AxesEntity.hpp>
#include <nimbus/Scene.hpp>
#include <nimbus/SimplePclConverter.hpp>

#include "PointCloud.hpp"


/**
 * Loads the provided depth frame into the point cloud, preparing the cloud_entity to draw it.
 * @param frame The depth frame to draw.
 * @param cloud The point cloud to store the points.
 * @param cloud_entity The cloud entity that will draw.
 */
void load_depth_frame_into_cloud(const DepthFrame& frame, PointCloud* cloud, PointXYZCloudEntity* cloud_entity) {
    static SimplePclConverter converter;
    converter.buffer_to_cloud(frame.data.data(), &cloud->points);
    cloud_entity->refresh_geometry();
}


void filter_cloud(Eigen::Vector3f *barbell, const PointCloud* cloud_in, PointCloud* cloud_out, PointXYZCloudEntity* cloud_entiry)
{

    auto pc = cloud_in->points;
    auto& pc_out = cloud_out->points;
    pc_out.clear();

    auto v0 = barbell[0];
    auto v1 = barbell[1];

    auto line = (v1 - v0).normalized();

    float threshold = 300.f / 1000.f;
    for(int i = 0; i < pc.size(); ++i)
    {
         auto p = pc.at(i).head<3>();
         auto distance_to_line = (p - v0).cross(line).norm();
         if( distance_to_line < threshold)
         {
             pc_out.emplace_back(pc.at(i));
         }
    }
    cloud_entiry->refresh_geometry();
    std::cout << "after filter: " << pc_out.size() << std::endl;

}

void separate_barbell_from_cloud(const PointCloud* cloud_in,  PointCloud* cloud_out, PointXYZCloudEntity* cloud_entiry, Eigen::Vector3f *barbell)
{
    auto pc = cloud_in->points;
    auto& pc_out = cloud_out->points;
    pc_out.clear();

    auto num_points = pc.size();
    const int max_iter = 2000;
    std::vector<int> indices_inliers;
    int max_num_inliers = 0;



    for(int i = 0; i < max_iter; ++i)
    {
        std::cout << "iteration:" << i << std::endl;
        auto index0 = rand() % num_points;
        auto index1 = rand() % num_points;
        auto v0 = pc.at(index0).head<3>();
        auto v1 = pc.at(index1).head<3>();

        auto line = (v1 - v0).normalized();
        if( abs(line.dot(Eigen::Vector3f(1.f, 0.f, 0.f))) < 0.90 ) // tj : assuming the barbell is almost horizontal
            continue;


        float radius_bar = 28.5f / 1000.f; // mm -> meter
        float radius_bell = 43.f / 1000.f;
        float height_bell = 20.f / 1000.f;

        std::vector<int> indices_inliers_temp;
        for(int j = 0; j < num_points; ++j)
        {
            auto p = pc.at(j).head<3>();
            auto distance_to_line = (p - v0).cross(line).norm();
            if( distance_to_line < radius_bar)
            {
                indices_inliers_temp.push_back(j);
                continue;
            }
//            else if( distance_to_line < radius_bell)
//            {
//                auto proj0 = abs((p - v0).dot(line)); // tj : project p-v0 along line
//                auto proj1 = abs((p - v1).dot(line));
//                if( proj0 < height_bell || proj1 < height_bell )
//                    indices_inliers_temp.push_back(j);
//            }
        }
        auto num_inliers = indices_inliers_temp.size();
        if( num_inliers > max_num_inliers)
        {
            max_num_inliers = num_inliers;
            indices_inliers_temp.swap(indices_inliers);
            barbell[0] = v0;
            barbell[1] = v1;
        }
    }// for
    auto cloud_iterator = cloud_out->points.begin();
    for (int index : indices_inliers)
    {
        pc_out.emplace_back(pc.at(index));
    }
    cloud_entiry->refresh_geometry();
}


int main(int argc, char** argv) {
    auto window = initialize_window(1280, 720);

    Scene scene; // contains all visible 3D objects and helps draw them.
    Eigen::Vector3f camera_position = {-0.5f, 0.5f, 0.0f};
    Eigen::Vector3f camera_target = {0.0f, 0.0f, 2.0f};
    Camera camera(Pose::LookAt(camera_position, camera_target));

    auto white = Color::Rgb(255, 255, 255);
    auto red = Color::Rgb(255, 0, 0);
    auto green = Color::Rgb(0, 255, 0);

    auto axes = std::make_shared<AxesEntity>();
    scene.add_entity(axes);

    auto point_cloud = std::make_shared<PointCloud>();
    auto point_cloud_entity = std::make_shared<PointXYZCloudEntity>(point_cloud);
    point_cloud_entity->set_color(white);
    scene.add_entity(point_cloud_entity);

    auto point_cloud_barbell = std::make_shared<PointCloud>();
    auto point_cloud_entity_barbell = std::make_shared<PointXYZCloudEntity>(point_cloud_barbell);
    point_cloud_entity_barbell->set_color(red);
    scene.add_entity(point_cloud_entity_barbell);


    auto point_cloud_filtered = std::make_shared<PointCloud>();
    auto point_cloud_entity_filtered = std::make_shared<PointXYZCloudEntity>(point_cloud_filtered);
    point_cloud_entity_filtered->set_color(green);
//    scene.add_entity(point_cloud_entity_filtered);




    std::ifstream ifs("/home/seamanj/Workspace/cloudviewer/recordings/1.dpv", std::ifstream::in);
    std::istream &is = ifs;
    dpv_skip_header(&is);
    DepthFrame df;

    srand (time(NULL));

    bool playing = false;
    constexpr auto system_min = std::chrono::system_clock::time_point::min();
    auto last_frame = system_min;
    std::chrono::duration<float, std::milli> duration {0.0F};

//    dpv_read_depth_frame(&is, &df);

    Eigen::Vector3f barbell[2];
    bool bFirstFrame = true;
    while(glfwWindowShouldClose(window) != 1) {
        update(scene);

        ImGui::Begin("Main Window");
        {
            // TODO: Your ImGui GUI code here
            if (ImGui::Button("Play"))
            {
                playing = true;
            }
            if (ImGui::Button("Pause"))
            {
                playing = false;
            }

        }
        ImGui::End();

        if(playing )//&& dpv_read_depth_frame(&is, &df)
        {
            dpv_read_depth_frame(&is, &df);
            if( last_frame != system_min)
                duration = df.timestamp - last_frame;
            last_frame = df.timestamp;
            load_depth_frame_into_cloud(df, point_cloud.get(), point_cloud_entity.get());
            if( !bFirstFrame )
            {
                filter_cloud(barbell, point_cloud.get(), point_cloud_filtered.get(), point_cloud_entity_filtered.get());
                separate_barbell_from_cloud(point_cloud_filtered.get(), point_cloud_barbell.get(), point_cloud_entity_barbell.get(), barbell );
            }
            else
            {
                separate_barbell_from_cloud(point_cloud.get(), point_cloud_barbell.get(), point_cloud_entity_barbell.get(), barbell);
                bFirstFrame = false;
            }
        }
        else {
            duration = std::chrono::duration<float, std::milli>::zero();
        }

        start_new_frame(window);
        {
            scene.render(camera);
            render(window);
            std::this_thread::sleep_for(duration);
        }
        finish_new_frame(window);
    }

    cleanup(window);
    return 0;
}
